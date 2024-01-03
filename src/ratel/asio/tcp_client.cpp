/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: tcp_client.cpp 
 *  Copyright (c) 2023-2024 scofieldzhu
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

#include "tcp_client.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "spdlog/spdlog.h"
#include "tcp_session.h"

namespace ba = boost::asio;
namespace ba_ip = boost::asio::ip;
using tcp_type = ba_ip::tcp;
using tcp_socket = tcp_type::socket;

RATEL_NAMESPACE_BEGIN

struct TcpClient::Impl 
{
    ba::io_context& io_context;
    bool async_mode;
    TcpClient* owner = nullptr;

    Impl(ASIO_CTX ctx, bool m)
        :io_context(*reinterpret_cast<ba::io_context*>(ctx)),
        async_mode(m)
    {
    }

    void connect(const std::string& server, short port)
    {
        tcp_type::resolver resolver(io_context);
        tcp_type::resolver::results_type endpoints = resolver.resolve(server, std::to_string(port));
        auto new_session = TcpSession::Create(&io_context, async_mode);
        auto self(new_session->shared_from_this());
        ba::async_connect(*reinterpret_cast<tcp_socket*>(self->socket()),
                          endpoints,
                          boost::bind(&TcpClient::Impl::onConnect, this, self, ba::placeholders::error)
        );
    }

    void onConnect(TcpSessionPtr new_session, const boost::system::error_code& error) 
    {
        if(!error) {
            owner->conn_signal.invoke(new_session, "no error");
        }else{
            spdlog::error("Connect failed, error:{}", error.message());
            owner->conn_signal.invoke(new_session, error.message());
        }
    }

    TcpSessionPtr synConnect(const std::string& server, short port, std::string* detail_err)
    {
        tcp_type::resolver resolver(io_context);
        tcp_type::resolver::results_type endpoints = resolver.resolve(server, std::to_string(port));
        TcpSessionPtr new_session = TcpSession::Create(&io_context, async_mode);
        try{
            ba::connect(*reinterpret_cast<tcp_socket*>(new_session->socket()), endpoints);
        }catch(const boost::system::error_code& ec){
            if(detail_err)
                *detail_err = ec.message();
            return TcpSessionPtr();
        }
        if(detail_err)
            *detail_err = "no error";
        new_session->start(); //avoid reference count to zero
        return new_session;        
    }
};

TcpClient::TcpClient(ASIO_CTX context, bool m)
    :impl_(new Impl(context, m))
{
    impl_->owner = this;
}

TcpClient::~TcpClient()
{

}

void TcpClient::connect(const std::string& server, short port)
{
    impl_->connect(server, port);
}

ASIO_CTX TcpClient::context()
{
    return impl_->async_mode ? &impl_->io_context : nullptr;
}

// void TcpClient::run()
// {
//     if(impl_->async_mode)
//         impl_->io_context.run();
// }

// void TcpClient::exit()
// {
//     if(impl_->async_mode)
//         impl_->io_context.stop();
// }

TcpSessionPtr TcpClient::syncConnect(const std::string &server, short port, std::string* detail_err)
{
    return impl_->synConnect(server, port, detail_err);
}

RATEL_NAMESPACE_END