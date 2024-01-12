/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: element.h  
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

#ifndef __element_proxy_hpp__
#define __element_proxy_hpp__

#include <type_traits>
#include "ratel/basic/string_proxy.h"

RATEL_NAMESPACE_BEGIN

template <typename T>
class ElementProxy
{
public:
    using value_type = std::enable_if_t<(std::is_arithmetic_v<T> && sizeof(T) >= 4), T>;
    using reference = value_type&;
    using const_reference = const value_type&;

    ByteVec serializeToBytes()const
    {
        ByteVec bv(sizeof(value_type), 0);
        memcpy(bv.data(), std::addressof(element_), sizeof(value_type));
        return bv;
    }

    size_t loadBytes(ConsBytePtr buffer, size_t size)
    {
        if(buffer == nullptr || size < sizeof(value_type))
            return 0;
        memcpy(std::addressof(element_), buffer, sizeof(value_type));
        return sizeof(value_type);
    }

    reference mutableElement(){ return std::ref(element_); }

    const_reference element()const{ return std::cref(element_); }
    
    ElementProxy(value_type v)
        :element_(v){}

private:
    value_type element_;
};

RATEL_NAMESPACE_END

#endif