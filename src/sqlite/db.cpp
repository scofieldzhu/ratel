/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqlitedb.cpp 
CreateTime: 2018-7-28 10:49
=========================================================================*/
#include "db.h"
#include "path.h"
#include "statement.h"
#include "sqlite3.h"
#include "sqlitelogger.h"

RATEL_NAMESPACE_BEGIN

DB::DB(void* conn)
    :dbconn_(conn)
{}

DB * DB::OpenDB(const Path& dbfile, int32 flags, const char* zvfs)
{
    sqlite3* conn = nullptr;
    int32 err = sqlite3_open_v2(dbfile.rstring().cstr(), &conn, flags, zvfs);
    if(err != SQLITE_OK){
        slog_err(sqlitelogger) << "sqlite3_open_v2 error:" << sqlite3_errmsg(conn) << std::endl;
        return nullptr;
    }
    return new DB(conn);
}

Statement* DB::createStatement(const RString& sql, const char** pztail /*= nullptr*/)
{
    sqlite3_stmt* res_stmt = nullptr;
    int32 err = sqlite3_prepare_v2((sqlite3*)dbconn_, sql.cstr(), (int)sql.size(), &res_stmt, pztail);
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

RString DB::errMsg()
{
    return sqlite3_errmsg((sqlite3*)dbconn_);
}

DB::~DB()
{
    sqlite3_close_v2((sqlite3*)dbconn_);
    dbconn_ = nullptr;
}

RATEL_NAMESPACE_END
