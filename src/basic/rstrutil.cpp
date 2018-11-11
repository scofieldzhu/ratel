/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: rstrutil.cpp 
CreateTime: 2018-9-24 20:50
=========================================================================*/
#include "rstrutil.h"
#include <objbase.h>

RATEL_NAMESPACE_BEGIN
RString rstrutil::NewGuid()
{
    GUID guid;
    ::CoCreateGuid(&guid);
    return RString::FormatString("%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
                                guid.Data1, guid.Data2, guid.Data3,
                                guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                                guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

RATEL_NAMESPACE_END