/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqlitecon.cpp 
CreateTime: 2019-11-3 21:07
=======================================================================*/
#include "sqlitecon.h"
#include "sqlitelogger.h"
#include "sqlite3.h"
#include "path.h"

RATEL_NAMESPACE_BEGIN

SqliteCon::SqliteCon()
{}

SqliteCon::~SqliteCon()
{}

void SqliteCon::InitWinTempDir(const Path& tmpdir)
{
    char zPathBuf[MAX_PATH + 1];
    memset(zPathBuf, 0, sizeof(zPathBuf));
    WideCharToMultiByte(CP_UTF8, 0, tmpdir.toWString().c_str(), -1, zPathBuf, sizeof(zPathBuf), NULL, NULL);
    sqlite3_temp_directory = sqlite3_mprintf("%s", zPathBuf);
}

bool SqliteCon::open(const Path& dbfile, int32_t flags, const char* zvfs /*= nullptr*/)
{
    if(isOpened()){
        slog_warn(sqlitelogger) << "closed existed connection!" << endl;
        close();
    }
    sqlite3* conn = nullptr; //sqlite3_temp_directory
    int32_t err = sqlite3_open_v2(dbfile.rstring().cstr(), &conn, flags, zvfs);
    if(err != SQLITE_OK){
        slog_err(sqlitelogger) << "sqlite3_open_v2 error:" << sqlite3_errmsg(conn) << std::endl;
        sqlite3_close(conn);
        handle_ = nullptr;
        return false;
    }
    handle_ = conn;
    return true;
}

void SqliteCon::close()
{
    if(isOpened()){
        if(sqlite3_close((sqlite3*)handle_) != SQLITE_OK){
            slog_err(sqlitelogger) << "sqlite3_close error:" << sqlite3_errmsg((sqlite3*)handle_) << std::endl;
            return;
        }
    }
    handle_ = nullptr;
}

RString SqliteCon::lastErrMsg() const
{
    return isClosed() ? "no err" : sqlite3_errmsg((sqlite3*)handle_);    
}

RATEL_NAMESPACE_END