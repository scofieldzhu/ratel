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
#include "sqlite3.h"

RATEL_NAMESPACE_BEGIN

typedef int(*DBCallback)(void*, int, char**, char**);

class RATEL_SQLITE_API DB
{
public:
    static DB* OpenDB(const RString& dbfile, int32 flags, const char* zvfs = nullptr);
    sqlite3_stmt* prepareStatement(const RString& sql, const char** pztail = nullptr);
    void destroyStatement(sqlite3_stmt* stmt);
    int32 stepExec(sqlite3_stmt* stmt);
    int32 exec(sqlite3_stmt* stmt, const RString& sql, DBCallback func, void* firstpara);
    sqlite3* handle() { return dbconn_; }    
    ~DB();

private:
    DB(sqlite3* conn);
    sqlite3* dbconn_ = nullptr;
};

RATEL_NAMESPACE_END

#endif
