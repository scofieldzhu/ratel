/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: dbtable.h 
CreateTime: 2018-7-28 10:38
=========================================================================*/
#ifndef __dbtable_h__
#define __dbtable_h__

#include "sqlitepublic.h"
#include <initializer_list>

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API DbTable
{
public:
    DbTable& addColumn(const DbTableCol& col);
    DbTable& removeColumn(const RString& colname);
    DbTableCol* getColumn(const RString& colname);
    DbTableCol* getColumn(int32 colidx);
    RString makeCreateSql(bool ifexists = true);    
    RString makeInsertSql(const char* valfmt, ...);
    RString makeInsertSql(std::initializer_list<const char*> fields, const char* valfmt, ...);
    RString makeDelRowWhenSql(const char* whenfmt, ...);
    RString makeQueryRowWhenSql(const char* whenfmt, ...);
    RString makeDropSql();
    DbTable(const RString& name);
    virtual ~DbTable();

private:
    const RString name_;
    std::vector<DbTableCol*> columns_;
};

RATEL_NAMESPACE_END

#endif
