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
#include "tcp_session.h"

namespace ba = boost::asio;
namespace ba_ip = boost::asio::ip;
using tcp_type = ba_ip::tcp;
using tcp_socket = tcp_type::socket;

RATEL_NAMESPACE_BEGIN

struct TcpClient::Impl 
{
    ba::io_context io_context;

    Impl()
    {
    }

    TcpSessionPtr connect(const std::string& server, short port, std::string* detail_err)
    {
        tcp_type::resolver resolver(io_context);
        tcp_type::resolver::results_type endpoints = resolver.resolve(server, std::to_string(port));
        TcpSessionPtr new_session = TcpSession::Create(&io_context);
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

TcpClient::TcpClient()
    :impl_(new Impl())
{

}

TcpClient::~TcpClient()
{

}

SCK_CTX TcpClient::context()
{
    return &impl_->io_context;
}

void TcpClient::run()
{
    impl_->io_context.run();
}

void TcpClient::exit()
{
    impl_->io_context.stop();
}

TcpSessionPtr TcpClient::connect(const std::string &server, short port, std::string *detail_err)
{
    return impl_->connect(server, port, detail_err);
}

RATEL_NAMESPACE_END