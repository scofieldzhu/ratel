/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: dbglo.h 
CreateTime: 2019-1-26 19:42
=========================================================================*/
#ifndef __dbglo_h__
#define __dbglo_h__

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

namespace sqlkw
{
    RATEL_SQLITE_API extern const RString kPrimaryKey;
    RATEL_SQLITE_API extern const RString kUnique;
    RATEL_SQLITE_API extern const RString kNotNull;
    RATEL_SQLITE_API extern const RString kDefault;
}


RATEL_NAMESPACE_END
#endif
