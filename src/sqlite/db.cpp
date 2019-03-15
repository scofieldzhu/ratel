/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqlitedb.cpp 
CreateTime: 2018-7-28 10:49
=========================================================================*/
#include "db.h"
#include "dbtable.h"
#include "path.h"
#include "statement.h"
#include "sqlite3.h"
#include "sqlitelogger.h"

RATEL_NAMESPACE_BEGIN

DB::DB(const Path& dbfile, int32_t flags, const char* zvfs)
	:dbfilepath_(dbfile)
{
	sqlite3* conn = nullptr;
	int32_t err = sqlite3_open_v2(dbfile.rstring().cstr(), &conn, flags, zvfs);
	if(err != SQLITE_OK)
		slog_err(sqlitelogger) << "sqlite3_open_v2 error:" << sqlite3_errmsg(conn) << std::endl;
	dbconn_ = conn;
}

DB::~DB()
{
	sqlite3_close_v2((sqlite3*)dbconn_);
	dbconn_ = nullptr;
}

Statement* DB::createStatement(const RString& sql, const char** pztail /*= nullptr*/)
{
    sqlite3_stmt* res_stmt = nullptr;
    int32_t err = sqlite3_prepare_v2((sqlite3*)dbconn_, sql.cstr(), (int)sql.size(), &res_stmt, pztail);
    if(err != SQLITE_OK){
        slog_err(sqlitelogger) << "sqlite3_prepare_v2 error:" << sqlite3_errmsg((sqlite3*)dbconn_) << std::endl;
        return nullptr;
    }
    return new Statement(res_stmt, this);
}

bool DB::exec(const RString& sql, StatCallback func, void* firstpara)
{
    char* errmsg = nullptr;
    sqlite3_exec((sqlite3*)dbconn_, sql.cstr(), func, firstpara, &errmsg);
    if(errmsg != nullptr){
        slog_err(sqlitelogger) << "sqlite3_exec error:" << (const char*)errmsg << std::endl;
        sqlite3_free(errmsg);
        return false;
    }
    return true;
}

bool DB::execUpdateData(const RString& sql)
{
	Statement* stat = createStatement(sql);
	if(stat == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_DONE){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		delete stat;
		return false;
	}
	delete stat;
	return true;
}

bool DB::execCommitData()
{
	return execUpdateData("COMMIT;");
}

bool DB::queryFirstRowResultData(const RString& sql, RowDataDict& resultdata)
{
	Statement* stat = createStatement(sql);
	if(stat == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_ROW){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		delete stat;
		return false;
	}
	stat->fetchDataDict(resultdata); 
	delete stat;
	return true;
}

bool DB::queryColumnValueOfFirstResultRow(const RString& sql, int32_t columnindex, Variant& result)
{
	Statement* stat = createStatement(sql);
	if(stat == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_ROW){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		delete stat;
		return false;
	}
	stat->fetchColumnData(columnindex, result);
	delete stat;
	return true;
}

bool DB::queryColumnValueOfFirstResultRow(const RString& sql, const RString& columnkey, Variant& result)
{
	Statement* stat = createStatement(sql);
	if(stat == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	if (rc != SQLITE_ROW){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		delete stat;
		return false;
	}
	stat->fetchColumnData(columnkey, result);
	delete stat;
	return true;
}

RString DB::errMsg()
{
    return sqlite3_errmsg((sqlite3*)dbconn_);
}

void DB::joinTable(DbTable& t)
{
	if(existTable(t.name())){
		slog_err(sqlitelogger) << "The table(" << t.name().cstr() << ") already exists!" << endl;
		return;
	}
	alltables_.push_back(&t);
}

void DB::dismissTable(const RString& tablename)
{
	auto it = std::find_if(alltables_.begin(),
						   alltables_.end(),
						   [&tablename](DbTable* t){return t->name() == tablename;});
	if(it != alltables_.end())
		alltables_.erase(it);
}

bool DB::existTable(const RString& tablename) const
{
	return std::find_if(alltables_.begin(), 
						alltables_.end(), 
						[&tablename](DbTable* t){return t->name() == tablename;}
						) != alltables_.end();
}

DbTable* DB::fetchTable(const RString& name)
{
	auto it = std::find_if(alltables_.begin(),
						   alltables_.end(),
						   [&name](DbTable* t){return t->name() == name;});
	return it != alltables_.end() ? *it : nullptr;
}

RATEL_NAMESPACE_END