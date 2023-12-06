/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: langext.h 
=========================================================================*/
#ifndef __langext_h__
#define __langext_h__

#include "ratel_nsp.h"

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
