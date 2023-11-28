/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqliteresult.h 
CreateTime: 2019-8-27 21:10
=======================================================================*/
#ifndef __sqliteresult_h__
#define __sqliteresult_h__

#include "sqlitepublic.h"
#include "sqlrecord.h"

RATEL_NAMESPACE_BEGIN

class SqliteCon;
class RATEL_SQLITE_API SqliteResult
{
public:    
    bool fetchNext();
    int32_t currentColumnCount();
    bool exec(const RString& sql);
    bool prepare(const RString& sql);
    bool finalize() ;
    void reset();
    void bindColumn();
    const SqlRecord& currentRecord()const{ return currecord_; }
    SqliteResult(SqliteCon& con);
    ~SqliteResult();

private:
    void updateRecord();
    SqliteCon& conn_;
    void* stmt_ = nullptr;
    SqlRecord currecord_;
};

RATEL_NAMESPACE_END

#endif
