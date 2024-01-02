/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: tcp_server.cpp 
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

#include "tcp_server.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "tcp_session.h"
#include "spdlog/spdlog.h"

namespace baip = boost::asio::ip;
using TcpSocket = baip::tcp::socket;
using IoContext = boost::asio::io_context;

RATEL_NAMESPACE_BEGIN

struct TcpServer::Impl 
{
    bool async_mode;
    IoContext io_context;
    std::unique_ptr<baip::tcp::acceptor> acceptor;
    TcpServer* owner = nullptr;

    void startNextAccept()
    {
        auto new_session = TcpSession::Create(&io_context, async_mode);
        TcpSocket& sck = *reinterpret_cast<TcpSocket*>(new_session->socket());
        acceptor->async_accept(sck, 
                               boost::bind(&TcpServer::Impl::handleAccept, this, new_session, boost::asio::placeholders::error)
        );
    }

    void handleAccept(TcpSessionPtr new_session, const boost::system::error_code& ec)
    {
        if(!ec){
            new_session->start();
            owner->conn_signal.invoke(new_session);
            if(!io_context.stopped())
                startNextAccept();
        }else{
            spdlog::error("Accept error:{}", ec.message());
            return;
        }         
    }

    Impl(short port, bool m)
        :async_mode(m)
    {
        acceptor = std::make_unique<baip::tcp::acceptor>(io_context, baip::tcp::endpoint(baip::tcp::v4(), port));
    }
};

TcpServer::TcpServer(short port, bool async_mode)
    :impl_(new Impl(port, async_mode))
{
    impl_->owner = this;
    impl_->startNextAccept();
}

TcpServer::~TcpServer()
{
}

SCK_CTX TcpServer::context()
{
    return impl_->async_mode ? &impl_->io_context : nullptr;
}

void TcpServer::run()
{
    if(impl_->async_mode)
        impl_->io_context.run();
}

void TcpServer::exit()
{
    if(impl_->async_mode)
        impl_->io_context.stop();
}

bool TcpServer::asynMode() const
{
    return impl_->async_mode;
}

RATEL_NAMESPACE_END