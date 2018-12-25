/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: langext.h 
CreateTime: 2018-12-16 22:01
=========================================================================*/
#ifndef __langext_h__
#define __langext_h__

#include "ratelnsp.h"

RATEL_NAMESPACE_BEGIN

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

RATEL_NAMESPACE_END

#define rtdelete(p) RATEL::CheckedDelete(p)
#define rtarrydelete(parry) RATEL::CheckedArrayDelete(p)

#endif
