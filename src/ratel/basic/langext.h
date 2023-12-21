/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: langext.h  
 *  Create Time: Tue Dec 19 2023
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

#ifndef __langext_h__
#define __langext_h__

#include "ratel/basic/ratel_nsp.h"
#include <cmath>

RATEL_NAMESPACE_BEGIN

typedef void* vptr_t;

constexpr const double kZeroValue = 1e-6;

template <typename T>
bool ValueEqual(const T& t1, const T& t2){ return t1 == t2; }
template <> inline bool ValueEqual<double>(const double& t1, const double& t2){ return fabs(t1 - t2) < kZeroValue; }
template <> inline bool ValueEqual<const char*>(const char* const& t1, const char* const& t2){return strcmp(t1, t2) == 0;}

template<typename T>
void CheckedDelete(T* p)
{
    #pragma warning(push)
    #pragma warning(disable:4101)
    char typeiscomplete[sizeof(T) ? 1 : -1];
    (void)sizeof(typeiscomplete);
    #pragma warning(pop)
    delete p;
}

template<typename T>
void CheckedArrayDelete(T* p)
{
    #pragma warning(push)
    #pragma warning(disable:4101)
    char typeiscomplete[sizeof(T) ? 1 : -1];
    (void)sizeof(typeiscomplete);
    #pragma warning(pop)
    delete[] p;
}

template <typename T>
void Ignore(T&&)
{}

RATEL_NAMESPACE_END

#define rtdelete(p) RATEL::CheckedDelete(p)
#define rtarrydelete(parry) RATEL::CheckedArrayDelete(parry)

#endif
