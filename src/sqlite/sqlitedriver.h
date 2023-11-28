/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqlitedriver.h 
CreateTime: 2019-8-19 21:51
=======================================================================*/
#ifndef __sqlitedriver_h__
#define __sqlitedriver_h__

#include "dbdriver.h"
#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API SqliteDriver : public DbDriver
{
public:
    bool open(const RString& db,
        const RString& user = RString(),
        const RString& password = RString(),
        const RString& host = RString(),
        int port = -1,
        const RString& connOpts = RString()) override;
    void close() override;
    ConHandle handle() override;
    bool beginTransaction() override;
    bool commitTransaction() override;
    bool rollbackTransaction() override;
    DbmsType dbmsType()const override;
    SqliteDriver();
    ~SqliteDriver();

private:
    ConHandle conhandle_;
};
RATEL_NAMESPACE_END

#endif
