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
#include "rowdatadict.h"
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
	int retcode = sqlite3_close_v2((sqlite3*)dbconn_);
	if(retcode != SQLITE_OK)
		slog_err(sqlitelogger) << "sqlite close failed! errcode:" << retcode << endl;
	dbconn_ = nullptr;
}

StatementSPtr DB::createStatement(const RString& sql, const char** pztail /*= nullptr*/)
{
    sqlite3_stmt* res_stmt = nullptr;
    int32_t err = sqlite3_prepare_v2((sqlite3*)dbconn_, sql.cstr(), (int)sql.size(), &res_stmt, pztail);
    if(err != SQLITE_OK){
        slog_err(sqlitelogger) << "sqlite3_prepare_v2 error:" << sqlite3_errmsg((sqlite3*)dbconn_) << std::endl;
        return nullptr;
    }
    return StatementSPtr(new Statement(res_stmt, this));
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
	StatementSPtr stat = createStatement(sql);
	if(stat.get() == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_DONE){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		return false;
	}
	return true;
}

void DB::beginTransaction()
{
	execUpdateData("BEGIN;");
}

bool DB::commit()
{
	return execUpdateData("COMMIT;");
}

void DB::rollback()
{
	execUpdateData("ROLLBACK;");
}

bool DB::queryFirstResultRowData(const RString& sql, RowDataDict& resultdata)
{
	StatementSPtr stat = createStatement(sql);
	if(stat.get() == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_ROW){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		return false;
	}
	stat->fetchDataDict(resultdata); 
	return true;
}

bool DB::queryMultiResultRowData(const RString& sql, std::vector<RowDataDict>& resrows, const RowDataDict& reference)
{
	StatementSPtr stat = createStatement(sql);
	if(stat.get() == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	while(rc == SQLITE_ROW){
		RowDataDict newrow(reference);
		stat->fetchDataDict(newrow);
		resrows.push_back(newrow);
		rc = stat->stepExec();
	};
	return true;
}

bool DB::queryColumnValueOfFirstResultRow(const RString& sql, int32_t columnindex, Variant& result)
{
	StatementSPtr stat = createStatement(sql);
	if(stat.get() == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_ROW){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		return false;
	}
	stat->fetchColumnData(columnindex, result);
	return true;
}

bool DB::queryColumnValueOfFirstResultRow(const RString& sql, const RString& columnkey, Variant& result)
{
	StatementSPtr stat = createStatement(sql);
	if(stat.get() == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	if (rc != SQLITE_ROW){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		return false;
	}
	stat->fetchColumnData(columnkey, result);
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