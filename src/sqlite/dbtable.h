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
	DbTableCol& addNewColumn(const RString& key);
    DbTable& removeColumn(const RString& key);
    DbTableCol* getColumn(const RString& key)const;
    DbTableCol* getColumn(int32_t colidx)const;	
	bool existsColumn(const RString& key)const { return getColumn(key) != nullptr; }
    RString makeCreateSql(bool ifexists = true);   
	RString makeInsertRowSql(const DbTableRecord& row);
    RString makeInsertRowSql(const char* valfmt, ...);
    RString makeInsertRowSql(std::initializer_list<const char*> fields, const char* valfmt, ...);
    RString makeDelRowWhenSql(const char* whenfmt, ...);
    RString makeQueryRowWhenSql(const char* whenfmt, ...);
    RString makeDropSql();
    DbTable(const RString& name);
    virtual ~DbTable();

private:
	bool checkTableRecordValidity(const DbTableRecord&)const;
    const RString name_;
    std::vector<DbTableCol*> columns_;
};

RATEL_NAMESPACE_END

#endif
