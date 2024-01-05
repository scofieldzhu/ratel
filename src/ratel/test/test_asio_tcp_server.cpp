/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: test_asio_tcp_server.cpp 
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
#include "test_asio_tcp_server.h"
#include "ratel/asio/tcp_server.h"
#include "ratel/asio/tcp_session.h"
#include "ratel/asio/context_driver.h"
#include "ratel/basic/dbg_tracker.h"
#include "spdlog/spdlog.h"
using namespace ratel;
using namespace std::chrono_literals;

static std::map<TcpSession*, int> sTalkCnt;

static void OnClosed(TcpSession* session, bool active)
{
    _AUTO_FUNC_TRACK_
    spdlog::trace("OnClosed, ecode:{}", active);
}

static void OnRcvMsg(TcpSession* session, ConsBytePtr data, std::size_t len)
{
    std::string rcv_msg((const char*)data);
    spdlog::trace("Receive msg:{}", rcv_msg);
    std::string echo_msg = std::format("Acked :{}", rcv_msg);
    session->send(echo_msg.c_str(), echo_msg.size());
    sTalkCnt[session]++;
    if(sTalkCnt[session] == 2)
        session->close();
}

static void OnConnected(TcpSessionPtr session)
{
    _AUTO_FUNC_TRACK_
    session->rcv_signal.bind(&OnRcvMsg);
    session->close_signal.bind(&OnClosed);
    std::string welcome_msg = "Welcome you, buddy!";
    session->send(welcome_msg.c_str(), welcome_msg.size());
    sTalkCnt[session.get()] = 0;
}

void TestCase_Asio_Tcp_Server_Async()
{
    _AUTO_FUNC_TRACK_
    auto ctx = CreateAsioContext();
    auto server = std::make_unique<TcpServer>(ctx, 1102);
    server->conn_signal.bind(&OnConnected);
    RunAsioContext(ctx);
    server = nullptr;
    DestroyAsioContext(ctx);
}

void TestCase_Asio_Tcp_Server()
{
    _AUTO_FUNC_TRACK_
    TestCase_Asio_Tcp_Server_Async();
}
