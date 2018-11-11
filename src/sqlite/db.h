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

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API DB
{
public:
    static DB* OpenDB(const Path& dbfile, int32 flags, const char* zvfs = nullptr);
    Statement* createStatement(const RString& sql, const char** pztail = nullptr);
    RString errMsg();
    ~DB();

private:
    friend class Statement;
    DB(void* conn);
    DB(const DB&) = delete;
    const DB& operator=(const DB&) = delete;
    void* dbconn_;
};

RATEL_NAMESPACE_END

#endif
