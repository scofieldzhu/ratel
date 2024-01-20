/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: is_serializable_type.h  
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

#ifndef __is_serializable_hpp__
#define __is_serializable_hpp__

#include <concepts>
#include "ratel/basic/base_type.h"

RATEL_NAMESPACE_BEGIN

template <typename T, typename B, typename CB>
concept IsSerializable = requires(T t1, const T t2, CB cb, size_t s)
{
    {t2.serializeToBytes()}->std::same_as<ByteVec>;
    {t1.loadBytes(cb, s)}->std::same_as<size_t>;
};

template <typename T, typename B, typename CB>
concept VecMemberSerializable = IsSerializable<T, B, CB> && requires(T t1, T t2)
{
    {t1 = std::move(t2)};
};

RATEL_NAMESPACE_END

#endif