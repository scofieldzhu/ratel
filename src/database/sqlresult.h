/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: sqlresult.h 
CreateTime: 2019-7-30 21:27
=======================================================================*/
#ifndef __sqlresult_h__
#define __sqlresult_h__

#include "databasepublic.h"

RATEL_NAMESPACE_BEGIN

class SqlRecord;
class SqlResult
{
public:   
    virtual bool fetchNext() = 0;
    virtual int32_t currentColumnCount() = 0;
    virtual bool exec(const RString& sql) = 0;
    virtual bool prepare(const RString& sql) = 0;
    virtual bool finalize() = 0;
    virtual const SqlRecord& currentRecord()const = 0;
    virtual ~SqlResult() = default;
};

RATEL_NAMESPACE_END

#endif
