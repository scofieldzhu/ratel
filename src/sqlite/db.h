/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: db.h 
CreateTime: 2018-7-28 10:49
=========================================================================*/
#ifndef __db_h__
#define __db_h__

#include "sqlitepublic.h"
#include "path.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API DB
{
public:
	StatementSPtr createStatement(const RString& sql, const char** pztail = nullptr);
    typedef int(*StatCallback)(void*, int, char**, char**);
    bool exec(const RString& sql, StatCallback func, void* firstpara);
	bool execUpdateData(const RString& sql);
	void beginTransaction();
	bool commit();
	void rollback();
	bool queryMultiResultRowData(const RString& sql, std::vector<RowDataDict>& resrows, const RowDataDict& reference);
	bool queryFirstResultRowData(const RString& sql, RowDataDict& resultdata);
	bool queryColumnValueOfFirstResultRow(const RString& sql, int32_t columnindex, Variant& result);
	bool queryColumnValueOfFirstResultRow(const RString& sql, const RString& columnkey, Variant& result);
    RString errMsg();
	void joinTable(DbTable& t);
	void dismissTable(const RString& tablename);
	bool existTable(const RString& tablename)const;
	DbTable* fetchTable(const RString& name);
	const Path& dbFilePath()const { return dbfilepath_; }
	DB(const Path& dbfile, int32_t flags, const char* zvfs = nullptr);
    virtual ~DB();

private:
    friend class Statement;    
	const Path dbfilepath_;
    void* dbconn_;
	std::vector<DbTable*> alltables_;
};

RATEL_NAMESPACE_END

#endif
