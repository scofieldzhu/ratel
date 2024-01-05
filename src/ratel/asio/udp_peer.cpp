/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: udp_peer.cpp 
 *  Copyright (c) 2024-2024 scofieldzhu
 *  
 *  MIT License
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "udp_peer.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "spdlog/spdlog.h"

namespace baip = boost::asio::ip;
using Udp = baip::udp;
using UdpSocket = Udp::socket;
using EndPoint = Udp::endpoint;
using IoContext = boost::asio::io_context;

RATEL_NAMESPACE_BEGIN

namespace 
{
    constexpr std::size_t kMaxBufferSize = 1024;
}

struct UdpPeer::Impl
{
    IoContext& io_context;
    UdpSocket socket;
    Byte read_buf[kMaxBufferSize] = {0};
    std::size_t read_bytes_size = 0;
    Byte write_buf[kMaxBufferSize] = {0};
    std::size_t write_bytes_size = 0;
    EndPoint remote_endpoint;
    UdpPeer* owner = nullptr;

    Impl(ASIO_CTX ctx, short port)
        :io_context(*reinterpret_cast<IoContext*>(ctx)),
        socket(io_context, EndPoint(Udp::v4(), port))
    {

    }

    ~Impl()
    {

    }

    IoContext* getIoContext()
    {
        return reinterpret_cast<IoContext*>(&socket.get_executor().context());
    }

    void onReceive(const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        read_bytes_size = bytes_transferred;
        read_buf[read_bytes_size] = 0;
        if(!error){
            owner->rcv_signal.invoke(owner, read_buf, read_bytes_size, remote_endpoint.address().to_string(), remote_endpoint.port());
            if(!getIoContext()->stopped())
                owner->startReceive();
        }else{
            spdlog::error("Receive error:{}", error.message());
        }
    }

    void handleSent(const Byte* data, const boost::system::error_code& ec, std::size_t size)
    {
        owner->sent_signal.invoke(data, size, ec.message());
    }
};

UdpPeer::UdpPeer(ASIO_CTX ctx, short port)
    :impl_(new Impl(ctx, port))
{
    impl_->owner = this;
}

UdpPeer::~UdpPeer()
{
}

ASIO_CTX UdpPeer::context()
{
    return &impl_->io_context;
}

void UdpPeer::startReceive()
{
    impl_->socket.async_receive_from(
        boost::asio::buffer(impl_->read_buf), 
        impl_->remote_endpoint,
        boost::bind(&UdpPeer::Impl::onReceive, 
                    this->impl_.get(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)
    );
}

int UdpPeer::sendTo(const Byte* data, std::size_t size, const std::string& remote_ip, short port)
{
    if(data == nullptr || size >= kMaxBufferSize || size == 0){
        spdlog::error("No data or invalid buffer size passed!");
        return 1;
    }
    EndPoint remote_endpoint(baip::address::from_string(remote_ip), port);
    memcpy(impl_->write_buf, data, size);
    impl_->write_bytes_size = size;
    impl_->socket.async_send_to(
        boost::asio::buffer(impl_->write_buf, size), 
        remote_endpoint,
        boost::bind(&UdpPeer::Impl::handleSent,
                    impl_.get(), 
                    impl_->write_buf,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)
    );
    return 0;
}

size_t UdpPeer::syncSendTo(const Byte* data, std::size_t size, const std::string& remote_ip, short port, std::string* err)
{
    if(data == nullptr || size == 0){
        spdlog::error("Invalid data passed!");
        return 1;
    }
    EndPoint remote_endpoint(baip::address::from_string(remote_ip), port);
    boost::system::error_code error;
    auto finish_bytes = impl_->socket.send_to(boost::asio::buffer(data, size), remote_endpoint, 0, error);
    if(err)
        *err = error.message();
    return finish_bytes;
}

std::size_t UdpPeer::syncRecvFrom(const std::string& remote_ip, short port)
{
    EndPoint remote_endpoint(baip::address::from_string(remote_ip), port);
    impl_->read_bytes_size = impl_->socket.receive_from(boost::asio::buffer(impl_->read_buf, kMaxBufferSize), remote_endpoint);
    impl_->read_buf[impl_->read_bytes_size] = 0;
    return impl_->read_bytes_size;
}

std::tuple<const Byte*, std::size_t> UdpPeer::getRcvBuffer()
{
    return std::make_tuple(impl_->read_buf, impl_->read_bytes_size);
}

std::tuple<std::string, short> UdpPeer::remoteHost() const
{
    return std::make_tuple(impl_->remote_endpoint.address().to_string(), impl_->remote_endpoint.port());
}

RATEL_NAMESPACE_END
