/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: database.h 
CreateTime: 2019-7-30 21:23
=======================================================================*/
#ifndef __database_h__
#define __database_h__

#include "sqlquery.h"

RATEL_NAMESPACE_BEGIN

class RATEL_DATABASE_API Connection
{
public:
    RStrings tables()const;
    bool open();
    bool open(const RString& user, const RString& pwd);
    bool isOpened()const;
    bool isValid()const;
    SqlQuery exec(const RString& sql);
    bool transaction();
    bool commit();
    bool rollback();    
    void close();
    bool isOpened()const;
    void setDatabaseName(const RString& name);
    const RString& databaseName()const { return databasename_; }
    void setUserName(const RString& username);
    const RString& userName()const { return username_; }
    void setPassword(const RString& pwd);
    const RString& password()const { return password_; }
    void setPort(int32_t port);
    int32_t port()const { return port_; }
    void setHostName(const RString& host);
    const RString& hostName()const { return hostname_; }
    void setConnectOptions(const RString& ops);
    const RString& connectOptions() const { return connectoptions_; }
    Connection();
    ~Connection();

private:
    RString databasename_;
    RString hostname_;
    RString username_;
    RString password_;
    int32_t port_ = -1;
    RString connectoptions_;
};

RATEL_NAMESPACE_END

#endif
