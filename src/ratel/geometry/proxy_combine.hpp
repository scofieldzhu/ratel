/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: proxy_combine.h  
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
#ifndef __proxy_combine_hpp__
#define __proxy_combine_hpp__

#include <iterator>
#include <algorithm>
#include "ratel/basic/base_type.h"
#include "ratel/geometry/is_serializable.hpp"

RATEL_NAMESPACE_BEGIN

template <class ProxyA, class ProxyB, bool Ref = false>
class ProxyCombine;

template <class ProxyA, class ProxyB>
class ProxyCombine<ProxyA, ProxyB, false>
{
public:
    static_assert(IsSerializable<ProxyA, BytePtr, ConsBytePtr>);
    static_assert(IsSerializable<ProxyB, BytePtr, ConsBytePtr>);

    static ByteVec SerializeToBytes(const ProxyA& pa, const ProxyB& pb)
    {
        ByteVec bv_a = pa.serializeToBytes();
        auto bv_b = pb.serializeToBytes();
        std::copy(bv_b.begin(), bv_b.end(), std::back_inserter(bv_a));
        return bv_a;
    }

    ByteVec serializeToBytes()const
    {
        return SerializeToBytes(pa_, pb_);
    }

    static size_t LoadBytes(ProxyA& pa, ProxyB& pb, ConsBytePtr byte_data, size_t size)
    {
        if(byte_data == nullptr)
            return 0;
        auto cur_data = byte_data;
        auto left_size = size;
        auto finish_bytes = pa.loadBytes(cur_data, left_size);
        if(finish_bytes == 0)
            return 0;
        cur_data += finish_bytes;
        left_size -= finish_bytes;
        finish_bytes = pb.loadBytes(cur_data, left_size);
        if(finish_bytes == 0)
            return 0;
        return size - left_size;
    }

    size_t loadBytes(ConsBytePtr byte_data, size_t size)
    {
        return LoadBytes(pa_, pb_, byte_data, size);
    }

    ProxyA& proxyA(){ return pa_; }
    
    ProxyB& proxyB(){ return pb_; }

    ProxyCombine()
    {};

    ProxyCombine(ProxyA&& a, ProxyB&& b)
        :pa_(std::move(a)),
        pb_(std::move(b))
    {}

    ~ProxyCombine()
    {};

private:
    ProxyA pa_;
    ProxyB pb_;
};

template <class ProxyA, class ProxyB>
class ProxyCombine<ProxyA, ProxyB, true>
{
public:
    static_assert(IsSerializable<ProxyA, BytePtr, ConsBytePtr>);
    static_assert(IsSerializable<ProxyB, BytePtr, ConsBytePtr>);

    ByteVec serializeToBytes()const
    {
        return ProxyCombine<ProxyA, ProxyB, false>::SerializeToBytes(pa_, pb_);
    }

    size_t loadBytes(ConsBytePtr byte_data, size_t size)
    {
        return ProxyCombine<ProxyA, ProxyB, false>::loadBytes(pa_, pb_, byte_data, size);
    }

    ProxyA& proxyA()
    {
        return pa_; 
    }
    
    ProxyB& proxyB()
    { 
        return pb_; 
    }
    
    ProxyCombine(ProxyA& a, ProxyB& b)
        :pa_(a),
        pb_(b)
    {}

    ~ProxyCombine()
    {}

private:
    ProxyA& pa_;
    ProxyB& pb_;
};

RATEL_NAMESPACE_END

#endif