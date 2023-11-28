/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqlitedriver.cpp 
CreateTime: 2019-8-19 21:51
=======================================================================*/
#include "sqlitedriver.h"
#include "sqlite3.h"

RATEL_NAMESPACE_BEGIN

namespace
{    
    const char* kBusyTimeoutTag = "SQLITE_BUSY_TIMEOUT=";
    const char* kReadOnlyTag = "SQLITE_OPEN_READONLY";
    const char* kOpenUriTag = "SQLITE_OPEN_URI";
    const char* kEnableSharedCacheTag = "SQLITE_ENABLE_SHARED_CACHE";
    const char kTagSeparator = ';';
}

SqliteDriver::SqliteDriver()
{}

SqliteDriver::~SqliteDriver()
{}

bool SqliteDriver::open(const RString& db, const RString& user /*= RString()*/, const RString& password /*= RString()*/, const RString& host /*= RString()*/, int port /*= -1*/, const RString& connOpts /*= RString()*/)
{
    if(isOpened())
        close();
    int32_t timeout = 5000;
    bool sharecached = false;
    bool readonly = false;
    bool openuri = false;
    for(const auto& op : connOpts.split(kTagSeparator)){
        if(op.startWith(kBusyTimeoutTag))
            timeout = op.substr(strlen(kBusyTimeoutTag)).toUInt32();
        else if(op == kReadOnlyTag)
            readonly = true;
        else if(op == kEnableSharedCacheTag)
            sharecached = true;
        else if(op == kOpenUriTag)
            openuri = true;
    }
    int32_t openflag = readonly ? SQLITE_OPEN_READONLY : (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    if(openuri)
        openflag |= SQLITE_OPEN_URI;
    sqlite3_enable_shared_cache(sharecached);
    sqlite3* conn = nullptr;
    if(sqlite3_open_v2(db.cstr(), &conn, openflag, nullptr) != SQLITE_OK){
        setLastError(sqlite3_errmsg(conn));
        setOpened(false);
        return false;
    }
    setOpened(true);
    resetLastError();
    conhandle_ = conn;
    return true;
}

void SqliteDriver::close()
{
    if(isOpened()){
        int retcode = sqlite3_close_v2((sqlite3*)conhandle_);
        if(retcode != SQLITE_OK)
            setLastError(RString::FormatString("sqlite close failed! errcode=%d.", retcode));
        conhandle_ = nullptr;
        setOpened(false);
    }    
}

ConHandle SqliteDriver::handle()
{
    return conhandle_;
}

bool SqliteDriver::beginTransaction()
{
/*    sqlite3_*/
    return false;
}

bool SqliteDriver::commitTransaction()
{
    return false;
}

bool SqliteDriver::rollbackTransaction()
{
    return false;
}

DbmsType SqliteDriver::dbmsType() const
{
    return DbmsType::kSQLite;
}
RATEL_NAMESPACE_END
