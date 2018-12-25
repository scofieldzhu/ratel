/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: statement.cpp 
CreateTime: 2018-11-7 21:05
=========================================================================*/
#include "statement.h"
#include "db.h"
#include "sqlite3.h"
#include "sqlitelogger.h"

RATEL_NAMESPACE_BEGIN

Statement::Statement(void* handle, DB* db)
    :stmthandle_(handle),
    dbinst_(db)
{
    logverify(sqlitelogger, stmthandle_ && dbinst_);
}

Statement::~Statement()
{
    if(sqlite3_finalize((sqlite3_stmt*)stmthandle_) != SQLITE_OK)
        slog_err(sqlitelogger) << "sqlite3_finalize failed!" << endl;    
}

int32_t Statement::stepExec()
{
    return sqlite3_step((sqlite3_stmt*)stmthandle_);
}

int32 Statement::fetchDataCount()
{
    return sqlite3_data_count((sqlite3_stmt*)stmthandle_);
}

int32 Statement::fetchIntColumn(int32 col)
{
    return sqlite3_column_int((sqlite3_stmt*)stmthandle_, col);
}

int64 Statement::fetchInt64Column(int32 col)
{
    return sqlite3_column_int64((sqlite3_stmt*)stmthandle_, col);
}

RString Statement::fetchTextColumn(int32 col)
{
    const int32 bytenum = sqlite3_column_bytes((sqlite3_stmt*)stmthandle_, col);
    const unsigned char* textbuf = sqlite3_column_text((sqlite3_stmt*)stmthandle_, col);
    char* content = new char[bytenum + 1];
    memcpy(content, textbuf, bytenum);
    content[bytenum] = '\0';
    RString restext(content);
    delete[]content;
    return restext;
}

double Statement::fetchDoubleColumn(int32 col)
{
    return sqlite3_column_double((sqlite3_stmt*)stmthandle_, col);
}

int32 Statement::reset()
{
    return sqlite3_reset((sqlite3_stmt*)stmthandle_);
}

RString Statement::errMsg()
{
    return sqlite3_errmsg((sqlite3*)(dbinst_->dbconn_));
}

RATEL_NAMESPACE_END