/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: sqlbase.h 
CreateTime: 2019-7-30 21:34
=======================================================================*/
#ifndef __sqlbase_h__
#define __sqlbase_h__

#include "databasepublic.h"

RATEL_NAMESPACE_BEGIN

enum class SqlDataType
{
    kInteger,
    kReal,
    kChars,
    kText,
    kBinary,
    kImage
};

RATEL_NAMESPACE_END

#endif

