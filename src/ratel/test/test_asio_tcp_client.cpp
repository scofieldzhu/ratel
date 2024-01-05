/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: test_asio_tcp_client.cpp 
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

#include "test_asio_tcp_client.h"
#include "ratel/asio/tcp_client.h"
#include "ratel/asio/tcp_session.h"
#include "ratel/asio/context_driver.h"
#include "ratel/basic/dbg_tracker.h"
#include "spdlog/spdlog.h"
using namespace ratel;
using namespace std::chrono_literals;

static void OnClosed(TcpSession* session, bool active)
{
    _AUTO_FUNC_TRACK_
    spdlog::trace("OnClosed, ecode:{}", active);
}

static void OnRcvMsg(TcpSession* session, ConsBytePtr data, std::size_t len)
{
    _AUTO_FUNC_TRACK_
    spdlog::trace("Receive msg:{}", std::string((const char*)data));
    session->send("Acked", strlen("Acked"));
    //StopAsioContext(session->context());
    session->close();
}

static void OnMsgSent(TcpSession* s, std::size_t size)
{
    _AUTO_FUNC_TRACK_
    spdlog::trace("OnMsgSent: msg_len:{}", size);
}

static void OnConnected(TcpSessionPtr session, std::string err)
{
    _AUTO_FUNC_TRACK_
    spdlog::trace("OnConnected: msg:{}", err);
    session->sent_signal.bind(&OnMsgSent);
    session->rcv_signal.bind(&OnRcvMsg);
    session->close_signal.bind(&OnClosed);
    std::string msg("wopappa");
    session->send(msg.data(), msg.size());
}

void TestCase_Asio_Tcp_Client_Async()
{
    _AUTO_FUNC_TRACK_
    auto ctx = CreateAsioContext();
    auto client = std::make_unique<TcpClient>(ctx);
    client->conn_signal.bind(&OnConnected);
    client->connect("127.0.0.1", 1102);
    RunAsioContext(ctx);
    client = nullptr;
    DestroyAsioContext(ctx);
}

void TestCase_Asio_Tcp_Client_Sync()
{
    _AUTO_FUNC_TRACK_
    auto ctx = CreateAsioContext();
    auto client = std::make_unique<TcpClient>(ctx);
    client->conn_signal.bind(&OnConnected);
    auto session = client->syncConnect("127.0.0.1", 1102);
    auto len = session->syncRead();
    auto tuple = session->getRcvBuffer();
    std::string rcv_msg = std::get<0>(tuple);
    spdlog::trace("Rcv msg:{}", rcv_msg);
    session = nullptr;
    //RunAsioContext(ctx);
    client = nullptr;
    DestroyAsioContext(ctx);
}

void TestCase_Asio_Tcp_Client()
{
    _AUTO_FUNC_TRACK_
    TestCase_Asio_Tcp_Client_Async();
    TestCase_Asio_Tcp_Client_Sync();
}
