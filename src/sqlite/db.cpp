/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqlitedb.cpp 
CreateTime: 2018-7-28 10:49
=========================================================================*/
#include "db.h"
#include "sqlitelogger.h"

RATEL_NAMESPACE_BEGIN

DB::DB(sqlite3* conn)
    :dbconn_(conn)
{}

DB * DB::OpenDB(const RString& dbfile, int32 flags, const char* zvfs)
{
    sqlite3* conn = nullptr;
    int32 err = sqlite3_open_v2(dbfile.cstr(), &conn, flags, zvfs);
    if(err != SQLITE_OK){
        slog_err(sqlitelogger) << "sqlite3_open_v2 error:" << sqlite3_errmsg(conn) << std::endl;
        return nullptr;
    }
    return new DB(conn);
}

sqlite3_stmt * DB::prepareStatement(const RString & sql, const char** pztail)
{
    sqlite3_stmt* res_stmt = nullptr;
    int32 err = sqlite3_prepare_v2(dbconn_, sql.cstr(), sql.size(), &res_stmt, pztail);
    if(err != SQLITE_OK){
        slog_err(sqlitelogger) << "sqlite3_prepare_v2 error:" << sqlite3_errmsg(dbconn_) << std::endl;
        return nullptr;
    }
    return res_stmt;
}

void DB::destroyStatement(sqlite3_stmt* stmt)
{
    sqlite3_finalize(stmt);
}

int32 DB::stepExec(sqlite3_stmt * stmt)
{
    return sqlite3_step(stmt);
}

int32 DB::exec(sqlite3_stmt* stmt, const RString& sql, DBCallback func, void* firstpara)
{
    char* errmsg = nullptr;
    sqlite3_exec(dbconn_, sql.cstr(), func, firstpara, &errmsg);
    if(errmsg != nullptr){
        slog_err(sqlitelogger) << "sqlite3_exec error:" << (const char*)errmsg << std::endl;
        sqlite3_free(errmsg);
        return 0;
    }
    return 1;
}

DB::~DB()
{
    sqlite3_close_v2(dbconn_);
    dbconn_ = nullptr;
}

RATEL_NAMESPACE_END
