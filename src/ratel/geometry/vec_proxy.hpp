/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.geometry
Module: vec_proxy.hpp
Create time: 2023/12/08 09:37:45
=========================================================================*/
#ifndef __vec_proxy_h__
#define __vec_proxy_h__

#include "base_type.h"
#include <concepts>

RATEL_NAMESPACE_BEGIN

template <typename T, typename B, typename CB>
concept serializable = requires(T t1, T t2, B b, CB cb, size_t s){
    T();
    {t1 = std::move(t2)};
    {T::GetByteSize()}->std::same_as<size_t>;
    {t1.serializeToBytes(b, s)}->std::same_as<size_t>;
    {t1.loadBytes(cb, s)}->std::same_as<size_t>;
};

template <class E>
class VecProxy
{
public:
    static_assert(serializable<E, BytePtr, ConsBytePtr>);
    using element_type = E;
    using list_type = std::vector<element_type>;

    size_t getSerializedByteSize()const
    {
        return E::GetByteSize() * list_.size() + sizeof(int);
    }

    ByteVec serializeToBytes()const
    {
        const size_t kTotalByteSize = getSerializedByteSize();
        ByteVec bv(kTotalByteSize, 0);
        int element_count = (int)list_.size();
        BytePtr p_data = bv.data();
        memcpy(p_data, &element_count, sizeof(int));
        p_data += sizeof(int);
        size_t finish_bytes = sizeof(int);
        for(const auto& v : list_){
            auto cur_size = v.serializeToBytes(p_data, bv.size() - finish_bytes);
            if(!cur_size)
                return ByteVec();
            finish_bytes += cur_size;
            p_data += cur_size;
        }
        return bv;
    }

    bool loadBytes(ConsBytePtr byte_data, size_t size)
    {
        const size_t kTotalByteSize = getSerializedByteSize();
        if(byte_data == nullptr || size < sizeof(int))
            return false;
        const BytePtr byte_cursor = byte_data;
        int element_count = 0;
        memcpy(&element_count, byte_cursor, sizeof(int));
        byte_cursor += sizeof(int);
        size_t left_size = size - sizeof(int);
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
