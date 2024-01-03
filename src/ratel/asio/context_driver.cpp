/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: context_driver.cpp 
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

#include "context_driver.h"
#include <boost/asio.hpp>

RATEL_NAMESPACE_BEGIN

using IoContext = boost::asio::io_context;
using IoContextPtr = std::shared_ptr<IoContext>;

static std::vector<IoContextPtr> sContextVec;

ASIO_CTX CreateAsioContext()
{
    IoContextPtr context = std::make_shared<IoContext>();
    sContextVec.push_back(context);
    return context.get();
}

void RunAsioContext(ASIO_CTX ctx)
{
    if(ctx == nullptr)
        return;
    auto it = std::find_if(sContextVec.begin(), sContextVec.end(), [&ctx](auto item){
        return item.get() == ctx;
    });
    if(it != sContextVec.end())
        (*it)->run();
}

bool IsAsioContextStopped(ASIO_CTX ctx)
{
    if(ctx == nullptr)
        return true;
    auto it = std::find_if(sContextVec.begin(), sContextVec.end(), [&ctx](auto item){
        return item.get() == ctx;
    });
    if(it != sContextVec.end())
        return (*it)->stopped();
    return true;
}

void StopAsioContext(ASIO_CTX ctx)
{
    if(ctx == nullptr)
        return;
    auto it = std::find_if(sContextVec.begin(), sContextVec.end(), [&ctx](auto item){
        return item.get() == ctx;
    });
    if(it != sContextVec.end())
        return (*it)->stop();
}

void DestroyAsioContext(ASIO_CTX ctx)
{
    if(ctx == nullptr)
        return;
    auto it = std::find_if(sContextVec.begin(), sContextVec.end(), [&ctx](auto item){
        return item.get() == ctx;
    });
    if(it != sContextVec.end())
        sContextVec.erase(it);
}

RATEL_NAMESPACE_END
