/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: connection.cpp 
CreateTime: 2019-7-30 21:23
=======================================================================*/
#include "connection.h"
#include "dbdriver.h"

RATEL_NAMESPACE_BEGIN

Connection::Connection(DbDriver& driver)
    :driver_(driver)
{}

Connection::~Connection()
{

}

RStrings Connection::tables() const
{
    return {"", ""};
}

bool Connection::open()
{
    if(!isOpened())
        return driver_.open(databasename_, username_, password_, hostname_, port_, connectoptions_);
    return false;
}

bool Connection::open(const RString& user, const RString& pwd)
{
    username_ = user;
    password_ = pwd;
    return open();
}

bool Connection::isOpened() const
{
    return driver_.isOpened();
}

bool Connection::isValid() const
{
    return isOpened();
}

// SqlQuery Connection::execQuery(const RString& sql)
// {
//     return SqlQuery("", nullptr);
// }

bool Connection::transaction()
{
    return isValid() ? driver_.beginTransaction() : false;    
}

bool Connection::commit()
{
    return isValid() ? driver_.commitTransaction() : false;
}

bool Connection::rollback()
{
    return isValid() ? driver_.rollbackTransaction() : false;
}

void Connection::close()
{
    if(isValid())
        driver_.close();
}

DbmsType Connection::dbmsType() const
{
    return driver_.dbmsType();
}
RATEL_NAMESPACE_END