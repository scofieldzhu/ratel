/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: directed_graph.h 
Create time: 2023/12/03 11:24:58
=========================================================================*/
#ifndef __id_seed_hpp__
#define __id_seed_hpp__

#include <limits>
#include <type_traits>
#include "ratel/basic/ratel_nsp.h"

RATEL_NAMESPACE_BEGIN

template <typename T>
class IdSeed
{
public:
	using id_type = typename std::enable_if<std::is_integral_v<T> && std::is_unsigned_v<T>, T>::type;
	
	id_type next()
	{
		if(seed_ == std::numeric_limits<id_type>::max()){
			seed_ = init_value_;
		}else{
			++seed_;
		}
		return seed_;
	}

	void force(id_type value)
	{
		seed_ = value;
	}

	id_type init_value()const
	{
		return init_value_;
	}

	explicit IdSeed(id_type init_value = id_type(0))
		:seed_(init_value),
		init_value_(init_value)
	{}

	~IdSeed() = default;

private:
	id_type seed_;
	id_type init_value_;
};


RATEL_NAMESPACE_END

#endif