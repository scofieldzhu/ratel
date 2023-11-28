/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: dbdriver.h 
CreateTime: 2019-7-30 21:26
=======================================================================*/
#ifndef __dbdriver_h__
#define __dbdriver_h__

#include "sqlbase.h"

RATEL_NAMESPACE_BEGIN

class DbDriver
{
public:
    virtual bool open(const RString& db, 
                      const RString& user = RString(), 
                      const RString& password = RString(), 
                      const RString& host = RString(), 
                      int port = -1,
                      const RString& connOpts = RString()) = 0;
    void setOpened(bool flag) { opened_ = flag; }  
    bool isOpened()const { return opened_; }
    void setLastError(const RString& err) { lasterr_ = err; }
    const RString& lastError()const { return lasterr_; }
    void resetLastError() { lasterr_ = NOERR; }      
    virtual void close() = 0;
    virtual ConHandle handle() = 0;
    virtual bool beginTransaction() = 0;
    virtual bool commitTransaction() = 0;
    virtual bool rollbackTransaction() = 0;
    virtual DbmsType dbmsType()const = 0;
    virtual ~DbDriver() = default;

private:
    bool opened_ = false;
    RString lasterr_ = NOERR;
};

RATEL_NAMESPACE_END

#endif
