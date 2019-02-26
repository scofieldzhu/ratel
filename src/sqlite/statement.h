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

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API Statement
{
public:    
    DB* db() { return dbinst_; }
    int32_t stepExec();
    int32_t fetchDataCount();
    int32_t fetchIntColumn(int32_t col);
    int64_t fetchInt64Column(int32_t col);
    RString fetchTextColumn(int32_t col);
    double fetchDoubleColumn(int32_t col);
    int32_t reset();
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
