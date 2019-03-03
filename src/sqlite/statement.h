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

#include "sqlitelogger.h"

RATEL_NAMESPACE_BEGIN

class RowDataDict;
class RATEL_SQLITE_API Statement
{
public:    
    DB* db() { return dbinst_; }
    int32_t stepExec();
    int32_t fetchColumnCount();
    int32_t fetchIntColumnData(int32_t col);
    RString fetchTextColumnData(int32_t col);
    double fetchDoubleColumnData(int32_t col);
	void fetchColumnData(int32_t column, Variant& data);
	void fetchColumnData(const RString& columnname, Variant& dt);
	void fetchDataDict(RowDataDict& dd);
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
