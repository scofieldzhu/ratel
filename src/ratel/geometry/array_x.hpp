/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: array_x.h  
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

#ifndef __array_x_hpp__
#define __array_x_hpp__

#include <type_traits>
#include <format>
#include <array>
#include <stdexcept>
#include <initializer_list>
#include "ratel/basic/base_type.h"

RATEL_NAMESPACE_BEGIN

template <typename T, size_t S>
class ArrayX final
{
public:
    static_assert(S);
    using value_type = std::enable_if_t<std::is_arithmetic_v<T>, T>;    
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;    
    static constexpr size_t N = S;    
    using array_type = value_type[N];
    static constexpr size_t ByteSize = sizeof(value_type) * N;

    size_t getByteSize()const
    {
        return ByteSize;
    }

    ByteVec serializeToBytes()const
    {
        ByteVec bv(ByteSize, 0);
        memcpy(bv.data(), (void*)&arry_, ByteSize);
        return bv;
    }

    size_t loadBytes(ConsBytePtr buffer, size_t size)
    {
        if(buffer == nullptr || size < ByteSize)
            return 0;
        memcpy((void*)&arry_, buffer, ByteSize);
        return ByteSize;
    }

    reference operator[](int idx)
    {
        if(idx >= N)
            throw std::out_of_range(std::format("Invalid index value:{}, array size:{}!", idx, N));
        return arry_[idx];
    }

    const_reference operator[](int idx)const
    {
        return const_cast<ArrayX&>(*this).operator[](idx);
    }

    std::array<value_type, N> toArray()const
    {
        return std::to_array(arry_);
    }

    pointer data()
    {
        return arry_;
    }

    const_pointer data()const
    {
        return arry_;
    }

    ArrayX& operator=(array_type a)
    {
        memcpy(arry_, a, ByteSize);
        return *this;
    }

    ArrayX& operator=(std::initializer_list<value_type> il)
    {
        for(auto i = 0; i < il.size(); ++i)
            arry_[i] = *(il.begin() + i);
        return *this;
    }

    ArrayX& operator=(const ArrayX& other)
    {
        if(this != std::addressof(other))
            memcpy(arry_, other.arry_, ByteSize);
        return *this;
    }

    ArrayX& operator=(ArrayX&& other)
    {
        arry_ = std::move(other.arry_);
        return *this;
    }

    ArrayX()
    {}

    ArrayX(value_type (&d)[N])
    {
        memcpy(arry_, d, ByteSize);
    }

    ArrayX(std::initializer_list<value_type> il)
    {
        *this = il;
    }

    ArrayX(const ArrayX& other)
    {
        if(this != std::addressof(other))
            memcpy(arry_, other.arry_, ByteSize);
    }

    ArrayX(ArrayX&& other)
    {
        *this = other;
    }

    ~ArrayX() = default;

private:
    array_type arry_ = {value_type(0)};
};

RATEL_NAMESPACE_END

#endif