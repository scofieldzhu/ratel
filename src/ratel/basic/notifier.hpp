/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: ratelnsp.h 
Create time: 2023/12/05 09:02:35
=========================================================================*/
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