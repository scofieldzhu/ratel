#include "test_asio.h"
/*
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *
 *  File: test_asio.cpp
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
#include "test_asio.h"
#include "ratel/asio/timer.h"
#include "ratel/asio/tcp_client.h"
#include "ratel/asio/tcp_server.h"
#include "ratel/asio/tcp_session.h"
#include "ratel/asio/context_driver.h"
#include "ratel/basic/dbg_tracker.h"
#include "spdlog/spdlog.h"
using namespace ratel;
using namespace std::chrono_literals;

int sTimeCount = 0;

void Timeout(Timer* t, int)
{
    spdlog::trace("Timeout... {}", sTimeCount++);
    if(sTimeCount == 20)
        t->stop();
}

void TestCase_Timer_Loop()
{
    _AUTO_FUNC_TRACK_
    auto context = CreateAsioContext();
    assert(context);
    auto timer = std::make_unique<Timer>(context, true);
    sTimeCount = 0;
    timer->timeout_signal.bind(&Timeout);
    timer->start(500ms);
    RunAsioContext(context);
    //DestroyAsioContext(context);
}

void TestCase_Asio()
{
    _AUTO_FUNC_TRACK_
    TestCase_Timer_Loop();
}
