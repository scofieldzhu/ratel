/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: timer.cpp 
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

#include "timer.h"
#include <boost/asio.hpp>
#include "spdlog/spdlog.h"

using IoContext = boost::asio::io_context;
using TimerType = boost::asio::steady_timer;

RATEL_NAMESPACE_BEGIN

struct Timer::Impl 
{
    bool single_shot = false;
    IoContext& io_context;
    std::chrono::milliseconds interval{0};
    TimerType timer_;
    bool asyn_mode;
    bool started_ = false;
    Timer* owner = nullptr;

    Impl(ASIO_CTX ctx, bool m)
        :io_context(*reinterpret_cast<IoContext*>(ctx)),
        timer_(io_context), //initialize sequence decided by variable declaration position.
        asyn_mode(m)
    {
    }

    ~Impl()
    {
    }

    void onTimeOut(const boost::system::error_code& ec)
    {
        owner->timeout_signal.invoke(ec.value());
        if(!owner->isStarted()) //stop at once
            return;
        if(owner->singleShot()) //just once
            return;
        timer_.expires_at(timer_.expiry() + interval);
        timer_.async_wait(std::bind(&Timer::Impl::onTimeOut, this, std::placeholders::_1));
    }
};

Timer::Timer(ASIO_CTX ctx, bool asyn_mode)
    :impl_(new Impl(ctx, asyn_mode))
{
    impl_->owner = this;
}

Timer::~Timer()
{
}

bool Timer::start(std::chrono::milliseconds interval, bool single_shot)
{
    if(isStarted()){
        spdlog::error("Timer is already started, please stop it!");
        return false;
    }
    impl_->interval = interval;
    impl_->single_shot = single_shot;
    impl_->started_ = true;
    impl_->timer_.expires_at(std::chrono::steady_clock::now() + impl_->interval);
    impl_->timer_.async_wait(std::bind(&Timer::Impl::onTimeOut, impl_.get(), std::placeholders::_1));
    return true;
}

std::chrono::milliseconds Timer::interval() const
{
    return impl_->interval;
}

bool Timer::singleShot() const
{
    return impl_->single_shot;
}

bool Timer::isStarted() const
{
    return impl_->started_;
}

void Timer::stop()
{
    impl_->started_ = false;
}

RATEL_NAMESPACE_END
