/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: tcp_session.cpp 
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

#include "tcp_session.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "spdlog/spdlog.h"
#include "ratel/basic/dbg_tracker.h"

namespace baip = boost::asio::ip;
using IoContext = boost::asio::io_context;

RATEL_NAMESPACE_BEGIN

constexpr std::size_t kMaxBufferSize = 1024;

struct TcpSession::Impl 
{
    baip::tcp::socket socket;
    Byte read_buf[kMaxBufferSize] = {0};
    std::size_t read_bytes_size = 0;
    Byte write_buf[kMaxBufferSize] = {0};
    std::size_t write_bytes_size = 0;
    bool async_mode;
    TcpSession* owner;
    
    Impl(ASIO_CTX ctx, bool m)
        :socket(*reinterpret_cast<IoContext*>(ctx)),
        async_mode(m)
    {
    }

    ~Impl()
    {        
    }

    IoContext* getIoContext()
    {
        return reinterpret_cast<IoContext*>(&socket.get_executor().context());
    }

    void postNextRead()
    {
        auto self(owner->shared_from_this());
        socket.async_read_some(
            boost::asio::buffer(read_buf, kMaxBufferSize),
            boost::bind(&TcpSession::Impl::onReadFinished, this, self, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }

    void onReadFinished(TcpSessionPtr self, const boost::system::error_code& ec, std::size_t len)
    {
        read_bytes_size = len;
        if(!ec){ //no error
            self->rcv_signal.invoke(self.get(), read_buf, len);
            if(!getIoContext()->stopped() && socket.is_open()) //trigger next read if not stopped            
                postNextRead();            
        }else if(ec == boost::asio::error::eof) {
            // ther other party close connection!
            self->close_signal.invoke(self.get(), false);
        }else{
            //other error occur
            auto msg = ec.message();
            spdlog::error("Error occur:{}", msg);
            self->err_signal.invoke(self.get(), msg);
        }
    }

    void postNextWrite()
    {
        auto self(owner->shared_from_this());
        boost::asio::async_write(socket, 
                                 boost::asio::buffer(write_buf, write_bytes_size),
                                 boost::bind(&TcpSession::Impl::onWriteFinished, this, self, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }

    void onWriteFinished(TcpSessionPtr self, const boost::system::error_code& ec, std::size_t len)
    {
        if(!ec){
            self->sent_signal.invoke(self.get(), len);
        }else if(ec == boost::asio::error::eof){
            // ther other party close connection!
            self->close_signal.invoke(self.get(), false);
        }else{
            //other error occur
            auto msg = ec.message();
            spdlog::error("Error occur:{}", msg);
            self->err_signal.invoke(self.get(), msg);
        }
    }

    std::size_t syncRead(std::string* detail_err)
    {
        boost::system::error_code error;
        std::size_t len = socket.read_some(boost::asio::buffer(read_buf, kMaxBufferSize), error);
        if(error == boost::asio::error::eof){ // Connection closed cleanly by peer
            if(detail_err)
                *detail_err = "connection closed by peer";
            return 0;
        }
        if(error){
            if(detail_err)
                *detail_err = error.message();
            return 0;
        }
        return len;
    }

    std::size_t syncSend(const Byte* data, std::size_t size, std::string* detail_err)
    {
        boost::system::error_code ec;
        auto finish_len = boost::asio::write(socket,
            boost::asio::const_buffer(data, size),
            boost::asio::transfer_all(),
            ec
        );
        if(ec){
            if(detail_err)
                *detail_err = ec.message();
            spdlog::error("sync write buffer data to socket stream failed! err:{}", ec.message());
            return 0;
        }
        return finish_len;
    }
};

TcpSession::TcpSession(ASIO_CTX ctx, bool asyn_mode)
    :impl_(new Impl(ctx, asyn_mode))
{
    impl_->owner = this;
}

TcpSession::~TcpSession()
{
    _AUTO_FUNC_TRACK_
    close();
}

TcpSessionPtr TcpSession::Create(ASIO_CTX ctx, bool asyn_mode)
{
    if(ctx == nullptr)
        return nullptr;
    return TcpSessionPtr(new TcpSession(ctx, asyn_mode));
}

void TcpSession::start()
{
    impl_->postNextRead();
}

int TcpSession::send(const Byte* data, std::size_t size)
{
    if(!impl_->async_mode){
        spdlog::error("Cannot call send method in synchorous mode!");
        return 2;
    }
    if(data == nullptr || size > kMaxBufferSize){
        spdlog::error("Empty data or too large size:{}", size);
        return 1;
    }
    if(!isOpened()){
        spdlog::error("Not opened yet!");
        return 0;
    }
    memcpy(impl_->write_buf, data, size);
    impl_->write_bytes_size = size;
    impl_->postNextWrite();
    return 0;
}

std::size_t TcpSession::syncSend(const Byte* data, std::size_t size, std::string* detail_err)
{
    if(impl_->async_mode){
        spdlog::error("Cannot call syncSend method in asynchorous mode!");
        return 2;
    }
    if(data == nullptr || size == 0){
        spdlog::error("Empty data passed");
        return 0;
    }
    if(!isOpened()){
        spdlog::error("Not opened yet!");
        return 0;
    }
    return impl_->syncSend(data, size, detail_err);
}

std::size_t TcpSession::GetMaxSendRcvSize()
{
    return kMaxBufferSize;
}

SCK_HANDLE TcpSession::socket()
{
    return &impl_->socket;
}

std::tuple<const Byte*, std::size_t> TcpSession::getRcvBuffer()
{
    return std::make_tuple(impl_->read_buf, impl_->read_bytes_size);
}

bool TcpSession::asynMode() const
{
    return impl_->async_mode;
}

std::size_t TcpSession::syncRead(std::string* detail_err)
{
    if(!impl_->async_mode){
        if(!isOpened()){
            spdlog::error("Not opened yet!");
            return 0;
        }
        return impl_->syncRead(detail_err);
    }
    return 0; // not in synchorous mode!
}

ASIO_CTX TcpSession::context()
{
    return impl_->getIoContext();
}

bool TcpSession::isOpened() const
{
    return impl_->socket.is_open();
}

void TcpSession::close()
{
    if(impl_->socket.is_open()){
        impl_->socket.close();
        close_signal.invoke(this, true);
    }
}

RATEL_NAMESPACE_END
