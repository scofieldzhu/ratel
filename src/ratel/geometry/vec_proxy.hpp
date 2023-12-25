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
#include "ratel/basic/base_type.h"
#include "ratel/geometry/is_serializable.hpp"

RATEL_NAMESPACE_BEGIN

template <class E>
class VecProxy
{
public:
    static_assert(IsSerializable<E, BytePtr, ConsBytePtr> || std::is_arithmetic_v<E>);
    using element_type = E;
    using list_type = std::vector<element_type>;
    static constexpr bool FixedSize = false;

    static constexpr size_t GetByteSize()
    {
        return 0;
    }

    size_t getByteSize()const
    {
        if constexpr(std::is_arithmetic_v<element_type>){
            return sizeof(element_type) * list_.size() + kUIntSize;
        }else{
            if constexpr(element_type::FixedSize){
                return E::GetByteSize() * list_.size() + kUIntSize;
            }else{
                size_t calc_size = kUIntSize;
                std::for_each(list_.begin(), list_.end(), [&calc_size](const auto& it){ calc_size += it.getByteSize(); });
                return calc_size;
            }
        }        
    }

    ByteVec serializeToBytes()const
    {
        const size_t kTotalByteSize = getByteSize();
        ByteVec bv(kTotalByteSize, 0);
        unsigned int element_count = (unsigned int)list_.size();
        BytePtr p_data = bv.data();
        memcpy(p_data, &element_count, kUIntSize);
        p_data += kUIntSize;
        size_t finish_bytes = kUIntSize;
        if constexpr(std::is_arithmetic_v<element_type>){
            memcpy(p_data, (void*)list_.data(), kTotalByteSize - kUIntSize);
        }else{
            for(const auto& v : list_){
                auto cur_size = v.serializeToBytes(p_data, bv.size() - finish_bytes);
                if(!cur_size)
                    return ByteVec();
                finish_bytes += cur_size;
                p_data += cur_size;
            }
        }
        return bv;
    }

    bool loadBytes(ConsBytePtr byte_data, size_t size)
    {
        if(byte_data == nullptr || size < kUIntSize)
            return false;
        auto byte_cursor = byte_data;
        unsigned int element_count = 0;
        memcpy(&element_count, byte_cursor, kUIntSize);
        byte_cursor += kUIntSize;
        size_t left_size = size - kUIntSize;
        if constexpr(std::is_arithmetic_v<element_type>){
            const size_t kTotalByteSize = getByteSize();
            list_.resize(element_count);
            memcpy((void*)list_.data(), byte_cursor, kTotalByteSize - kUIntSize);
        }else{
            list_.clear();
            for(auto i = 0; i < element_count; ++i){
                element_type e;
                auto finish_size = e.loadBytes(byte_cursor, left_size);
                if(finish_size == 0)
                    return false;
                byte_cursor += finish_size;
                left_size -= finish_size;
                list_.push_back(std::move(e));
            }
        }
        return true;
    }

    list_type& mutableData()const
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

    ~VecProxy()
    {}

private:
    list_type list_;
};

RATEL_NAMESPACE_END

#endif // __vec_proxy_h__
