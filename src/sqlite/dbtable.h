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

RATEL_NAMESPACE_BEGIN

class DB;
class RATEL_SQLITE_API DbTable
{
public:	
	enum RowState
	{
		NORMAL,
		REMOVED
	};
	DbTableCol& addNewColumn(const RString& key);
    DbTable& removeColumn(const RString& key);
    DbTableCol* getColumn(const RString& key)const;
    DbTableCol* getColumn(int32_t colidx)const;	
	bool existsColumn(const RString& key)const { return getColumn(key) != nullptr; }
    RString makeCreateSql(bool ifexists = true);   
	RString makeInsertRowSql(const DbTableRecord& row);    
    RString makeDelRowWhenSql(const char* whenfmt, ...);
    RString makeQueryRowWhenSql(const char* whenfmt, ...);
    RString makeDropSql();
	bool create();
	bool insertRow(const DbTableRecord& record);
	int32_t queryPrimaryKeyId(const RString& sql);
	void drop();
	void setDB(DB* db) { db_ = db; }
	DB* db(){ return db_; }
    DbTable(const RString& name, DB* db = nullptr);
    virtual ~DbTable();

protected:
	bool checkTableRecordValidity(const DbTableRecord&)const;
    const RString name_;
    std::vector<DbTableCol*> columns_;
	DB* db_;
};

RATEL_NAMESPACE_END

#endif
