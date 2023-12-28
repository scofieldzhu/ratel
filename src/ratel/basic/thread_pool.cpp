/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: thread_pool.cpp 
 *  Copyright (c) 2023-2023 scofieldzhu
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

#include "thread_pool.h"

RATEL_NAMESPACE_BEGIN

ThreadPool::ThreadPool(size_t number)
{
	for(auto i = 0; i < number; ++i){
		workers_.emplace_back([this]{
			while(true){
				task_func active_task;
				{
					std::unique_lock lk(mutex_);
					cv_.wait(lk, [this]{ 
						return this->stop_ || !this->pending_tasks_.empty();
					});
					if(this->stop_ && this->pending_tasks_.empty())
						return;
					active_task = std::move(this->pending_tasks_.front());
					this->pending_tasks_.pop();
				}
				active_task();
			};
		});
	}
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lk(mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for(std::thread& w : workers_)
        w.join();
}

void ThreadPool::enqueue(task_func&& t)
{
	{
		std::unique_lock<std::mutex> lk(mutex_);
		pending_tasks_.emplace(std::move(t));
	}
	cv_.notify_one();
}

RATEL_NAMESPACE_END