 /*
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: vec3.hpp  
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
#ifndef __vec3_hpp__
#define __vec3_hpp__

#include "vec2.hpp"

RATEL_NAMESPACE_BEGIN

template <typename T>
class Vec3
{
public:
    using value_type = std::enable_if_t<std::is_arithmetic_v<T> && sizeof(T) >= 4, T>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using array_type = value_type[3];
    static constexpr size_t kArrayByteSize = sizeof(value_type) * 3;

    static constexpr size_t GetByteSize()
    {
        return kArrayByteSize;
    }

    size_t serializeToBytes(BytePtr buffer, size_t size)const
    {
        if(buffer == nullptr || size < kArrayByteSize)
            return 0;
        memcpy(buffer, (void*)&arry_, kArrayByteSize);
        return kArrayByteSize;
    }

    size_t loadBytes(ConsBytePtr buffer, size_t size)
    {
        if(buffer == nullptr || size < kArrayByteSize)
            return 0;
        memcpy((void*)&arry_, buffer, kArrayByteSize);
        return kArrayByteSize;
    }

    reference operator[](int idx)
    {
        if(idx != 0 && idx != 1 && idx != 2)
            throw std::out_of_range("Invalid index value, only 0\1\2 allowed!");
        return arry_[idx];
    }

    const_reference operator[](int idx)const
    {
        return dynamic_cast<Vec3&>(*this).operator[](idx);
    }

    std::array<value_type, 3> toArray()const
    {
        return std::to_array(arry_);
    }

    value_type* data()
    {
        return arry_;
    }

    const value_type* data()const
    {
        return arry_;
    }

    Vec3& operator=(array_type a)
    {
        arry_[0]= a[0];
        arry_[1]= a[1];
        arry_[2]= a[2];
        return *this;
    }

    Vec3& operator=(std::initializer_list<value_type> il)
    {
        if(il.size() < 3)
            return *this;
        arry_[0] = *(il.begin());
        arry_[1] = *(++il.begin());
        arry_[2] = *(++il.begin());
        return *this;
    }

    Vec3& operator=(Vec3&& other)
    {
        arry_[0] = std::move(other.arry_[0]);
        arry_[1] = std::move(other.arry_[1]);
        arry_[2] = std::move(other.arry_[2]);
        return *this;
    }

    Vec3& operator=(const Vec3& other)
    {
        arry_[0] = other.arry_[0];
        arry_[1] = other.arry_[1];
        arry_[2] = other.arry_[2];
        return *this;
    }

    Vec3()
    {}

    Vec3(const Vec2<value_type>& other, value_type v)
    {
        arry_[0] = other[0];
        arry_[1] = other[1];
        arry_[2] = v;
    }

    Vec3(value_type v1, value_type v2, value_type v3)
    {
        arry_[0] = v1;
        arry_[1] = v2;
        arry_[2] = v3;
    }

    Vec3(Vec3&& other)
    {
        arry_[0] = std::move(other.arry_[0]);
        arry_[1] = std::move(other.arry_[1]);
        arry_[2] = std::move(other.arry_[2]);
    }

    Vec3(const Vec3& other)
    {
        arry_[0] = other.arry_[0];
        arry_[1] = other.arry_[1];
        arry_[2] = other.arry_[2];
    }

    ~Vec3() = default;

private:
    array_type arry_ = {value_type(0)};
};

RATEL_NAMESPACE_END

#endif
