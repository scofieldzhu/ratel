/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: statement.h 
CreateTime: 2018-11-7 20:55
=========================================================================*/
#ifndef __statement_h__
#define __statement_h__

#include "rescode.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API Statement
{
public:
    typedef int(*StatCallback)(void*, int, char**, char**);
    int32 exec(const RString& sql, StatCallback func, void* firstpara);
    DB* db() { return dbinst_; }
    ResultCode stepExec();
    int32 fetchDataCount();
    int32 fetchIntColumn(int32 col);
    int64 fetchInt64Column(int32 col);
    RString fetchTextColumn(int32 col);
    double fetchDoubleColumn(int32 col);
    int32 reset();
    RString errMsg();
    ~Statement();

private:
    friend class DB;
    Statement(void* handle, DB* dbinst);
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;
    void* stmthandle_;
    DB* dbinst_;
};

RATEL_NAMESPACE_END

#endif
