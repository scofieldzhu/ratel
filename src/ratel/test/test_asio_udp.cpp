/*
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *
 *  File: test_asio_udp.cpp
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
#include "test_asio_udp.h"
#include "ratel/asio/udp_peer.h"
#include "ratel/asio/context_driver.h"
#include "ratel/basic/dbg_tracker.h"
#include "spdlog/spdlog.h"
using namespace ratel;
using namespace std::chrono_literals;

static void OnSent(ConsBytePtr data, std::size_t size, std::string err)
{
    std::string msg(data, size);
    spdlog::trace("Msg:{} is sent! err:{}", msg, err);
}

static void OnRecv(UdpPeer* udp_peer, ConsBytePtr data, std::size_t size, std::string remote_ip, short port)
{
    std::string msg(data, size);
    spdlog::trace("Recv msg:{} removte:{}:{}", msg, remote_ip, port);
    if(msg == "Exit"){
        StopAsioContext(udp_peer->context());
    }else{
        std::string echo_msg = std::format("Acked msg:{}", msg);
        udp_peer->sendTo(echo_msg.data(), echo_msg.size(), remote_ip, port);
    }
}

void TestCase_Asio_Udp_Async()
{
    _AUTO_FUNC_TRACK_
    auto ctx = CreateAsioContext();
    auto udp_peer = std::make_unique<UdpPeer>(ctx, 2334);
    udp_peer->rcv_signal.bind(&OnRecv);
    udp_peer->sent_signal.bind(&OnSent);
    udp_peer->startReceive();
    std::string msg = "Welcome udp peer";
    udp_peer->sendTo(msg.data(), msg.size(), "127.0.0.1", 5555);
    RunAsioContext(ctx);
    udp_peer = nullptr;
    DestroyAsioContext(ctx);
}

void TestCase_Asio_Udp_Sync()
{
    _AUTO_FUNC_TRACK_
    auto ctx = CreateAsioContext();
    auto udp_peer = std::make_unique<UdpPeer>(ctx, 2334);
    std::string remote_ip = "127.0.0.1";
    short port = 5555;
    std::string msg = "Welcome udp peer";
    udp_peer->syncSendTo(msg.data(), msg.size(), remote_ip, port);
    auto size = udp_peer->syncRecvFrom(remote_ip, port);
    auto data_tuple = udp_peer->getRcvBuffer();
    std::string remote_msg(std::get<0>(data_tuple), std::get<1>(data_tuple));
    spdlog::trace("Recv Msg:{} size:{}", remote_msg, size);
    udp_peer = nullptr;
    DestroyAsioContext(ctx);
}

void TestCase_Asio_Udp()
{
    _AUTO_FUNC_TRACK_
    TestCase_Asio_Udp_Async();
    TestCase_Asio_Udp_Sync();
}
