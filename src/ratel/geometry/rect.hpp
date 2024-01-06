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

#include "ratel/geometry/array_x.hpp"

RATEL_NAMESPACE_BEGIN

template <typename T>
class Rect
{
public:
	using value_type = std::enable_if_t<std::is_same_v<T, int> || std::is_same_v<T, float>, T>;
	using point_type = ArrayX<value_type, 2>;
	using size_type = ArrayX<value_type, 2>;
	static constexpr size_t ByteSize = point_type::ByteSize + size_type::ByteSize + kFloatSize;

    ByteVec serializeToBytes()const
    {
		ByteVec bv(ByteSize, 0);
		auto lt_bv = lt_.serializeToBytes();
		auto cur_data = bv.data();
		memcpy(cur_data, lt_bv.data(), lt_bv.size());
		cur_data += lt_bv.size();
		auto s_bv = size_.serializeToBytes();
		memcpy(cur_data, s_bv.data(), s_bv.size());
		cur_data += s_bv.size();
        memcpy(cur_data, (void*)&angle_, kFloatSize);
        return bv;
    }

    size_t loadBytes(ConsBytePtr buffer, size_t size)
    {
        if(buffer == nullptr || size < ByteSize)
            return 0;
		auto left_size = size;
		auto cur_buffer = buffer;
        auto finish_bytes = lt_.loadBytes(cur_buffer, left_size);
		left_size -= finish_bytes;
		cur_buffer += finish_bytes;
		finish_bytes = size_.loadBytes(cur_buffer, left_size);
		left_size -= finish_bytes;
		cur_buffer += finish_bytes;
		memcpy((void*)&angle_, cur_buffer, kFloatSize);
		left_size -= kFloatSize;
		cur_buffer += kFloatSize;
        return size - left_size;
    }

	void setLeftTop(const point_type& pt){ lt_ = pt; }
	const point_type& lt()const{ return lt_; }
	void setSize(const size_type& s){ size_ = s; }
	const auto& size()const{ return size_; }
	void setAngle(float a){ angle_ = a; }
	auto angle()const{ return angle_; }

	Rect(const point_type& pt, const size_type& size, float a)
		:lt_(pt),
		size_(size),
		angle_(a)
	{}

	Rect(value_type x, value_type y, value_type w, value_type h, float a)
		:lt_({x, y}),
		size_({w, h}),
		angle_(a)
	{}

private:
	point_type lt_{value_type(0), value_type(0)};
	size_type size_{value_type(0), value_type(0)};
	float angle_ = 0.0f;
};

RATEL_NAMESPACE_END

#endif