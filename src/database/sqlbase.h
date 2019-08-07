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

#include "basic.h"

RATEL_NAMESPACE_BEGIN

enum class SqlDataType
{
    kInvalid,
    kInteger,
    kReal,
    kChars,
    kText,
    kBinary,
    kImage
};

enum class SqlStatement
{
    kUpdate,
    kSelect,
    kDelete,
    kInsert,
    kWhere
};

enum class DbmsType
{
    kSQLite,
    kMySql,
    kOracle,
    kSqlServer,
    kDB2
};

using ConHandle = void*;

RATEL_NAMESPACE_END

#endif

