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

#include <type_traits>
#include <algorithm>
#include <iterator>
#include "ratel/basic/base_type.h"
#include "ratel/geometry/is_serializable.hpp"

RATEL_NAMESPACE_BEGIN

template <class E>
concept VecProxyMember = IsSerializable<E, BytePtr, ConsBytePtr> || std::is_arithmetic_v<E>;

template <class E>
class VecProxy
{
public:
    static_assert(VecProxyMember<E>);
    using element_type = E;
    using list_type = std::vector<element_type>;

    ByteVec serializeToBytes()const
    {
        if constexpr(std::is_arithmetic_v<element_type>){
            size_t calc_size = sizeof(element_type) * list_.size() + kUIntSize;
            ByteVec bv(calc_size, 0);
            unsigned int element_count = (unsigned int)list_.size();
            memcpy(bv.data(), &element_count, kUIntSize);
            if(element_count)
                memcpy(bv.data() + kUIntSize, (void*)list_.data(), calc_size - kUIntSize);
            return bv;
        }else{
            ByteVec bv(kUIntSize, 0);
            unsigned int element_count = (unsigned int)list_.size();
            memcpy(bv.data(), &element_count, kUIntSize);
            for(const auto& v : list_){
                auto mbv = v.serializeToBytes();
                if(mbv.empty())
                    return {};
                std::copy(mbv.begin(), mbv.end(), std::back_inserter(bv));
            }
            return bv;
        }
    }

    size_t loadBytes(ConsBytePtr byte_data, size_t size)
    {
        if(byte_data == nullptr || size < kUIntSize)
            return 0;
        auto byte_cursor = byte_data;
        unsigned int element_count = 0;
        memcpy(&element_count, byte_cursor, kUIntSize);
        byte_cursor += kUIntSize;
        size_t left_size = size - kUIntSize;
        list_.clear();
        if constexpr(std::is_arithmetic_v<element_type>){
            if(element_count){
                list_.resize(element_count);
                memcpy((void*)list_.data(), byte_cursor, element_count * sizeof(element_type));
                left_size -= element_count * sizeof(element_type);
            }
        }else{            
            for(unsigned int i = 0; i < element_count; ++i){
                element_type e;
                auto finish_size = e.loadBytes(byte_cursor, left_size);
                if(finish_size == 0)
                    return 0;
                byte_cursor += finish_size;
                left_size -= finish_size;
                list_.push_back(std::move(e));
            }
        }
        return size - left_size;
    }

    list_type& mutableData()
    {
        return list_;
    }

    const list_type& data()const
    {
        return list_;
    }
    
    VecProxy()
    {}

    VecProxy(list_type& list)
        :list_(list)
    {}

    VecProxy(list_type&& list)
        :list_(std::move(list))
    {}

    ~VecProxy()
    {}

private:
    list_type list_;
};

RATEL_NAMESPACE_END

#endif // __vec_proxy_h__
