/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: dict_proxy.h  
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

#ifndef __dict_proxy_hpp__
#define __dict_proxy_hpp__

#include <map>
#include "ratel/basic/string_proxy.h"
#include "ratel/geometry/vec_proxy.hpp"

RATEL_NAMESPACE_BEGIN

template <typename T>
concept ArithStringType = std::is_arithmetic_v<T> || std::same_as<T, StringProxy>;

template <class K, class V>
class DictProxy 
{
public:
    static_assert(ArithStringType<K> && IsSerializable<V>);
    using key_type = K;
    using key_vec_proxy_type = VecProxy<key_type>;
    using value_type = V;
    using value_vec_proxy_type = VecProxy<value_type>;
    using map_type = std::map<key_type, value_type>;
    static constexpr bool FixedSize = false;

    static constexpr size_t GetByteSize()
    {
        return 0;
    }

    size_t getByteSize()const
    {
        size_t keys_byte_size = 0;
        size_t vals_byte_size = 0;
        for(const auto& kv : map_){
            if constexpr(key_type::FixedSize){
                keys_byte_size += key_type::GetByteSize();
            }else{
                keys_byte_size += kv.first.GetByteSize();
            }
            if constexpr(value_type::FixedSize){
                vals_byte_size += value_type::GetByteSize();
            }else{
                vals_byte_size += kv.second.GetByteSize();
            }
        }
        return keys_byte_size + vals_byte_size + kUIntSize * 2; // number and keys_byte_size
    }

    ByteVec serializeToBytes()const
    {
        key_vec_proxy_type keys;
        value_vec_proxy_type values;
        for(const auto& kv : map_){
            keys.mutableData().push_back(kv.first);
            values.mutableData().push_back(kv.second);
        }
        ByteVec bv_k = keys.serializeToBytes();
        ByteVec bv_v = values.serializeToBytes();
        size_t total_size = bv_k.size() + bv_v.size() + kUIntSize * 2;
        ByteVec bv(total_size);
        unsigned int number = map_.size();
        auto cur_data = bv.data();
        memcpy(cur_data, &number, kUIntSize);
        cur_data += kUIntSize;
        unsigned int key_byte_size = bv_k.size();
        memcpy(cur_data, &key_byte_size, kUIntSize);
        cur_data += kUIntSize;
        if(key_byte_size){
            memcpy(cur_data, bv_k.data(), bv_k.size());
            cur_data += bv_k.size();
            memcpy(cur_data, bv_v.data(), bv_v.size());
        }
        return bv;
    }

    bool loadBytes(ConsBytePtr byte_data, size_t size)
    {
        if(byte_data == nullptr || size < kUIntSize)
            return false;
        map_.clear();
        auto number = *(const unsigned int*)(byte_data);
        if(number == 0){            
            return true;
        }
        auto cur_data = byte_data + kUIntSize;
        auto left_size = size - kUIntSize;
        unsigned int key_byte_size = 0;
        memcpy(&key_byte_size, cur_data, kUIntSize);
        cur_data = byte_data + kUIntSize;
        left_size = size - kUIntSize;
        key_vec_proxy_type keys;
        if(!keys.loadBytes(cur_data, key_byte_size))
            return false;
        left_size -= key_byte_size;
        cur_data += key_byte_size;
        value_vec_proxy_type values;
        if(!values.loadBytes(cur_data, left_size))
            return false;     
        for(auto i = 0 ; i< number; ++i)
            map_.insert({std::move(keys[i]), std::move(values[i])});
        return true;
    }

    map_type& mutableData()const
    {
        return map_;
    }

    const map_type& data()const
    {
        return map_;
    }
    
    DictProxy()
    {}

    DictProxy(map_type& m)
        :map_(m)
    {}

    ~DictProxy()
    {}

private:
    map_type map_;
};

RATEL_NAMESPACE_END

#endif