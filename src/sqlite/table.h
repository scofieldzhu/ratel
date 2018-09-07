/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: table.h 
CreateTime: 2018-7-28 10:38
=========================================================================*/
#ifndef __table_h__
#define __table_h__

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API Table
{
public:
    Table& addColumn(const TableCol& col);
    Table& removeColumn(const RString& colname);
    TableCol* getColumn(const RString& colname);
    TableCol* getColumn(int32 colidx);
    RString makeCreateSql(bool ifexists = true);
    RString makeInsertSql(const char* format, ...);
    RString makeDelRowWhenSql(const char* whenfmt, ...);
    RString makeQueryRowWhenSql(const char* whenfmt, ...);
    RString makeDropSql();
    Table(const RString& name);
    ~Table();

private:
    const RString name_;
    std::vector<TableCol*> columns_;
};

RATEL_NAMESPACE_END

#endif
