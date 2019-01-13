/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: inttabcol.cpp 
CreateTime: 2019-1-13 20:41
=========================================================================*/
#include "inttabcol.h"

RATEL_NAMESPACE_BEGIN
IntTabCol::IntTabCol(const RString& name) 
    :DbTableCol(name, INTEGER)
{}

RString IntTabCol::description() const
{
    RString desp = DbTableCol::description();
    if(!default_)
        return desp;
    return RString::FormatString("%s %s %d", desp.cstr(), DEFAULT, defaultvalue_);
}

IntTabCol& IntTabCol::setDefaultValue(int32 val)
{
    defaultvalue_ = val;
    return *this;
}

RATEL_NAMESPACE_END