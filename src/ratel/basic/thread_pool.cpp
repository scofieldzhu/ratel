/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: thread_pool.cpp
Create time: 2023/12/07 20:35:45
=========================================================================*/
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