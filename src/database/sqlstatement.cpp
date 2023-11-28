/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: sqlstatement.cpp 
CreateTime: 2019-8-27 20:10
=======================================================================*/
#include "sqlstatement.h"

RATEL_NAMESPACE_BEGIN

SqlStatement::SqlStatement(const RString& rawsql, const char placeholder /*= '?'*/)
    :rawsql_(rawsql)
{
    RStrings substrs = rawsql.split(placeholder);
    if(substrs.size() >= 2){
        arguments_.resize(substrs.size() - 1);
        int32_t curindex = 0;
        for(auto i = 0; i < substrs.size() - 1; ++i){
            curindex += substrs[i].size();
            arguments_[curindex].pos = curindex;
        }
    }        
}

SqlStatement::~SqlStatement()
{
}

RString SqlStatement::formatSql()
{
    if(arguments_.empty())
        return rawsql_;
    RString resultsql = rawsql_;
    int32_t curindex = -1;
    while(++curindex < arguments_.size()){
        Argument& arg = arguments_[curindex];
        if(arg.pos >= 0 && !arg.value.isNull()){
            RString strval = arg.value.toStr();
            resultsql.replace(arg.pos, strval.size(), strval);
        }
    }
    return resultsql;
}

SqlStatement& SqlStatement::bindValue(const Variant& v, int32_t pos)
{
    if(pos >= 0 && pos < arguments_.size())
        arguments_[pos].value = v;        
    else if(pos == -1){
        //auto search next null value pos
        while(++pos < arguments_.size() && !arguments_[pos].value.isNull())
            arguments_[pos].value = v;
    }
    return *this;
}

RATEL_NAMESPACE_END
