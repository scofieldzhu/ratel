/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqlitepublic.h 
CreateTime: 2018-7-14 10:36
=========================================================================*/
#ifndef __sqlitepublic_h__
#define __sqlitepublic_h__

#include "filesyspublic.h"

#ifdef RATEL_SQLITE_EXPORTS
	#define RATEL_SQLITE_API __declspec(dllexport)
#else
    #define RATEL_SQLITE_API __declspec(dllimport)
#endif

RATEL_NAMESPACE_BEGIN
class DB;
class DbTable;
class DbTableCol;
class RowDataDict;
class Statement;
using StatementSPtr = std::shared_ptr<Statement>;
class SqlDataMeta;
class SqliteDriver;
RATEL_NAMESPACE_END

#endif
