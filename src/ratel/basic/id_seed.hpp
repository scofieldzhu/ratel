 /*
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: id_seed.hpp  
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