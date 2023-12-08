/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: thread_pool.hpp 
Create time: 2023/12/07 17:23:45
=========================================================================*/
#ifndef __thread_pool_hpp__
#define __thread_pool_hpp__

#include <condition_variable>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include "basic_export.h"

RATEL_NAMESPACE_BEGIN

class RATEL_BASIC_API ThreadPool
{
public:
	using task_func = std::function<void()>;
	void enqueue(task_func&& t);
	explicit ThreadPool(size_t number);
	~ThreadPool();

private:
	std::queue<task_func> pending_tasks_;
	std::vector<std::thread> workers_;
	std::condition_variable cv_;
	std::mutex mutex_;
	bool stop_ = false;
};

RATEL_NAMESPACE_END

#endif