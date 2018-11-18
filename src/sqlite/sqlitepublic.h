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

#include "basic.h"
#include "filesyspublic.h"

#ifdef RATEL_SQLITE_EXPORTS
    #define RATEL_SQLITE_API __declspec(dllexport)
#else
    #define RATEL_SQLITE_API __declspec(dllimport)
#endif

RATEL_NAMESPACE_BEGIN

enum DataType
{
    INTEGER,
    REAL,
    STRING
};

class TableRow;
class TableCol;
class Table;
class DB;
class Statement;

#define PRIMARYKEY "PRIMARY KEY"
#define UNIQUE "UNIQUE"
#define NOTNULL "NOT NULL"
#define DEFAULT "DEFAULT"

RATEL_NAMESPACE_END

#endif
