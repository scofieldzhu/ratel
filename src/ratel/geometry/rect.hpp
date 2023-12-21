 /*
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: rect.hpp  
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
#ifndef __rect_hpp__
#define __rect_hpp__

#include "vec2.hpp"

RATEL_NAMESPACE_BEGIN

template <typename T>
class Rect
{
public:
	using value_type = std::enable_if_t<std::is_same_v<T, int> || std::is_same_v<T, float>, T>;
	using point_type = Vec2<value_type>;
	using size_type = Vec2<value_type>;

	void setLT(const point_type& pt){ lt_ = pt; }
	const point_type& lt()const{ return lt_; }
	void setSize(const size_type& s){ size_ = s; }
	const auto& size()const{ return size_; }
	auto angle()const{return angle_;}

	Rect()
	{}

	~Rect() = default;

private:
	point_type lt_{value_type(0), value_type(0)};
	size_type size_{value_type(0), value_type(0)};
	float angle_ = 0.0f;
};

RATEL_NAMESPACE_END

#endif