/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: strtabcol.cpp 
CreateTime: 2019-1-13 20:54
=========================================================================*/
#include "strtabcol.h"

RATEL_NAMESPACE_BEGIN

StrTabCol::StrTabCol(const RString& name, int32 maxchars)
    :DbTableCol(name, STRING),
    maxcharnum_(maxchars)
{}

const char* StrTabCol::dataTypeString() const
{
    return "VARCHAR";
}

DbTableCol* StrTabCol::clone() const
{
    return new StrTabCol(*this);
}

RString StrTabCol::description() const
{
    RString desp = RString::FormatString("%s VARCHAR(%d)", name().cstr(), maxcharnum_);
    if(primaryKey()){
        desp += " ";
        desp += PRIMARYKEY;
    }
    if(unique()){
        desp += " ";
        desp += UNIQUE;
    }
    if(isNotNull()){
        desp += " ";
        desp += NOTNULL;
    }
    if(default_){
        desp += " ";
        desp += RString::FormatString("%s '%s'", DEFAULT, defaultvalue_.cstr());
    }
    return desp;
}

RATEL_NAMESPACE_END
