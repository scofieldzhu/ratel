/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: arraryx.hpp 
=========================================================================*/
#ifndef __arrayx_hpp__
#define __arrayx_hpp__

#include "basetypes.h"
RATEL_NAMESPACE_BEGIN

#define THROW_OUTOF_BOUND

template<typename T, size_t Size>
class ArrayX 
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    reference operator[](size_t idx){ return data_[idx]; }
    const_reference operator[](size_t idx)const { return data_[idx]; }
    reference at(size_t idx) // with bound check
    {
        if(idx < 0 || idx >= Size)
            throw std::invalid_argument("out of range");
        return data_[idx];
    }
    const_reference at(size_t idx)const{ return const_cast<ArrayX&>(*this).at(idx); }
    size_t size()const { return Size; }        
    pointer data() { return data_; }
    const_pointer data()const { return data_; }    
    void assign(const value_type a[Size])
    {
        for(size_t i = 0; i < Size; ++i)
            data_[i] = a[i];
    }
    ArrayX& operator=(const ArrayX& rhs)
    {
        for(size_t idx = 0; idx < Size; ++idx)
            data_[idx] = rhs.data_[idx];
        return *this;
    }
    ArrayX()
    {
        for (reference v : data_)
            v = T();
    }
    ArrayX(const value_type a[Size])
    {
        for(size_t i = 0; i < Size; ++i)
            data_[i] = a[i];
    }
    ArrayX(std::initializer_list<value_type> l)
    {
        const size_t kFinalSize = l.size() > Size ? Size : l.size();
        size_t i = 0;
        for(auto e : l) {
            if(i < kFinalSize)
                data_[i++] = e;
        }          
    }
    ~ArrayX()
    {}

protected:
    value_type data_[Size];
};

RATEL_NAMESPACE_END
#endif
