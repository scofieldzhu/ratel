 /*
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: vec_proxy.hpp  
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
#ifndef __vec_proxy_h__
#define __vec_proxy_h__

#include <iterator>
#include <algorithm>
#include <type_traits>
#include "ratel/basic/string_proxy.h"
#include "ratel/geometry/is_serializable.hpp"

RATEL_NAMESPACE_BEGIN

template <class E>
concept VecProxyMember = (VecMemberSerializable<E, BytePtr, ConsBytePtr> && !std::same_as<E, StringProxy>) || std::is_arithmetic_v<E> || std::same_as<E, std::string>;

template <class E, bool Ref = false>
class VecProxy;

template <class E>
class VecProxy<E, false>
{
public:
    static_assert(VecProxyMember<E>, "E is not allowed element type for VecProxy!");
    static constexpr bool kRef = false;
    using element_type = E;
    using element_reference = E&;
    using list_type = std::vector<E>;
    using list_reference = std::vector<E>&;
    using list_const_reference = const std::vector<E>&;

    static ByteVec SerializeToBytes(list_const_reference list)
    {
        if constexpr(std::is_arithmetic_v<element_type>){
            size_t calc_size = sizeof(element_type) * list.size() + kUIntSize;
            ByteVec bv(calc_size, 0);
            unsigned int element_count = (unsigned int)list.size();
            memcpy(bv.data(), &element_count, kUIntSize);
            if(element_count)
                memcpy(bv.data() + kUIntSize, (void*)list.data(), calc_size - kUIntSize);
            return bv;
        }else{
            ByteVec bv(kUIntSize, 0);
            unsigned int element_count = (unsigned int)list.size();
            memcpy(bv.data(), &element_count, kUIntSize);
            for(const auto& v : list){
                ByteVec mbv;
                if constexpr(std::is_same_v<element_type, std::string>){
                    mbv = StringProxy(v).serializeToBytes();
                }else{
                    mbv = v.serializeToBytes();
                }
                std::copy(mbv.begin(), mbv.end(), std::back_inserter(bv));
            }
            return bv;
        }
    }

    ByteVec serializeToBytes()const
    {
        return SerializeToBytes(list_);
    }

    static size_t LoadBytes(list_reference list, ConsBytePtr byte_data, size_t size)
    {
        if(byte_data == nullptr || size < kUIntSize)
            return 0;
        auto byte_cursor = byte_data;
        unsigned int element_count = 0;
        memcpy(&element_count, byte_cursor, kUIntSize);
        byte_cursor += kUIntSize;
        size_t left_size = size - kUIntSize;
        list.clear();
        if constexpr(std::is_arithmetic_v<element_type>){
            if(element_count){
                list.resize(element_count);
                memcpy((void*)list.data(), byte_cursor, element_count * sizeof(element_type));
                left_size -= element_count * sizeof(element_type);
            }
        }else{            
            for(unsigned int i = 0; i < element_count; ++i){
                element_type e;
                size_t finish_size = 0;
                if constexpr(std::is_same_v<element_type, std::string>){
                    StringProxy strp;
                    finish_size = strp.loadBytes(byte_cursor, left_size);
                    e = strp.stdStr();
                }else{
                    finish_size = e.loadBytes(byte_cursor, left_size);
                }
                if(finish_size == 0)
                    return 0;
                byte_cursor += finish_size;
                left_size -= finish_size;
                list.push_back(std::move(e));
            }
        }
        return size - left_size;
    }

    size_t loadBytes(ConsBytePtr byte_data, size_t size)
    {
        return LoadBytes(list_, byte_data, size);
    }

    list_reference mutableData()
    {
        return list_;
    }

    list_const_reference data()const
    {
        return list_;
    }
    
    VecProxy()
    {}

    ~VecProxy()
    {}

private:
    list_type list_;
};

template <class E>
class VecProxy<E, true>
{
public:
    static_assert(VecProxyMember<E>, "E is not allowed element type for VecProxy!");
    static constexpr bool kRef = true;
    using element_type = E;
    using element_reference = E&;
    using list_reference = std::vector<E>&;
    using list_const_reference = const std::vector<E>&;

    ByteVec serializeToBytes()const
    {
        return VecProxy<E, false>::SerializeToBytes(list_);
    }

    size_t loadBytes(ConsBytePtr byte_data, size_t size)
    {
        return VecProxy<E, false>::loadBytes(list_, byte_data, size);
    }

    list_reference mutableData()
    {
        return list_;
    }

    list_const_reference data()const
    {
        return list_;
    }
    
    VecProxy(list_reference list)
        :list_(list)
    {}

    ~VecProxy()
    {}

private:
    list_reference list_;
};

RATEL_NAMESPACE_END

#endif // __vec_proxy_h__
