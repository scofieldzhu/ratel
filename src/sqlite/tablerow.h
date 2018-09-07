/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: tablerow.h 
CreateTime: 2018-7-25 21:27
=========================================================================*/
#ifndef __tablerow_h__
#define __tablerow_h__

#include "sqlitepublic.h"
#include "sqlite3.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API TableRow
{
public:
    RString queryText(int32 col);
    int32 queryInteger(int32 col);
    double queryReal(int32 col);
    TableRow(sqlite3_stmt* st);
    ~TableRow();

private:
    sqlite3_stmt* stmt_;
};
RATEL_NAMESPACE_END
#endif
