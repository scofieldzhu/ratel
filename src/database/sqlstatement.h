/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: sqlstatement.h 
CreateTime: 2019-8-22 21:48
=======================================================================*/
#ifndef __sqlstatement_h__
#define __sqlstatement_h__

#include "databasepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_DATABASE_API SqlStatement
{
public:
    SqlStatement& bindValue(const Variant& v, int32_t pos = -1);
    const RString& rawSql()const { return rawsql_; }
    RString formatSql();
    SqlStatement(const RString& rawsql, const char placeholder = '?');
    ~SqlStatement();

private:
    struct Argument
    {
        int32_t pos = -1;
        Variant value;
    };
    const RString rawsql_;
    std::vector<Argument> arguments_;
};

RATEL_NAMESPACE_END

#endif
