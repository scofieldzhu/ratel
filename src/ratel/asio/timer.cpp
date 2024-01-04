/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: timer.cpp 
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

#include "timer.h"
#include <boost/asio.hpp>
#include "spdlog/spdlog.h"

using IoContext = boost::asio::io_context;
using TimerType = boost::asio::steady_timer;

RATEL_NAMESPACE_BEGIN

struct Timer::Impl 
{
    IoContext& io_context;
    std::chrono::milliseconds interval{0};
    TimerType timer;
    bool started = false;
    Timer* owner = nullptr;

    Impl(ASIO_CTX ctx)
        :io_context(*reinterpret_cast<IoContext*>(ctx)),
        timer(io_context) //initialize sequence decided by variable declaration position.
    {
    }

    ~Impl()
    {
    }

    void onTimeOut(const boost::system::error_code& ec)
    {
        owner->timeout_signal.invoke(owner, ec.value());
        if(!owner->isStarted()) //stop at once
            return;
        timer.expires_at(timer.expiry() + interval);
        timer.async_wait(std::bind(&Timer::Impl::onTimeOut, this, std::placeholders::_1));
    }
};

Timer::Timer(ASIO_CTX ctx)
    :impl_(new Impl(ctx))
{
    impl_->owner = this;
}

Timer::~Timer()
{
}

bool Timer::start(std::chrono::milliseconds interval)
{
    if(isStarted()){
        spdlog::error("Timer is already started, please stop it!");
        return false;
    }
    impl_->interval = interval;
    impl_->started = true;
    impl_->timer.expires_at(std::chrono::steady_clock::now() + impl_->interval);
    impl_->timer.async_wait(std::bind(&Timer::Impl::onTimeOut, impl_.get(), std::placeholders::_1));
    return true;
}

std::chrono::milliseconds Timer::interval() const
{
    return impl_->interval;
}

bool Timer::isStarted() const
{
    return impl_->started;
}

void Timer::stop()
{
    impl_->started = false;
}

void Timer::synOneShot(std::chrono::milliseconds milsec)
{
    TimerType t(impl_->io_context, milsec);
    t.wait();
}

RATEL_NAMESPACE_END
