 /*
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: notifier.hpp  
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
#ifndef __notifier_h__
#define __notifier_h__

#include <map>
#include <mutex>
#include <cstdint>
#include <functional>
#include "ratel/basic/id_seed.hpp"

RATEL_NAMESPACE_BEGIN

template <class... Ts>
class Notifier
{
public:
	using callback_type = std::function<void(Ts...)>;

	uint32_t bind(callback_type cb)
	{
		std::lock_guard auto_lock(lock_);
		uint32_t id = next_slot_id_.next();
		callback_map_[id] = cb;
		return id;
	}

	void unbind(uint32_t slot_id)
	{
		std::lock_guard auto_lock(lock_);
		callback_map_.erase(slot_id);
	}

	void clear()
	{
		std::lock_guard auto_lock(lock_);
		callback_map_.clear();
		next_slot_id_.force(0);
	}

	template <class... Args>
	void invoke(Args&&... args)
	{
		std::lock_guard auto_lock(lock_);
		for(const auto& kv : callback_map_){
			(kv.second)(std::forward<Ts>(args)...);
		}

	}

	template <class... Ts>
	void invokeR(Ts&... args)
	{
		std::lock_guard auto_lock(lock_);
		for(const auto& kv : callback_map_){
			(kv.second)(std::forward<Ts>(args)...);
		}
	}

	Notifier& operator=(const Notifier&) = delete;	
	Notifier& operator=(Notifier&&) = delete;
	
	Notifier() = default;
	Notifier(Notifier&&) = delete;
	Notifier(const Notifier&) = delete;
	~Notifier() = default;

private:
	std::map<uint32_t, callback_type> callback_map_;
	IdSeed<uint32_t> next_slot_id_{0};
	std::mutex lock_;
};

RATEL_NAMESPACE_END

#endif