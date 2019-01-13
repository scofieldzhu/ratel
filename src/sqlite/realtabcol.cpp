/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: realtabcol.cpp 
CreateTime: 2019-1-13 20:43
=========================================================================*/
#include "realtabcol.h"

RATEL_NAMESPACE_BEGIN

RealTabCol::RealTabCol(const RString& name)
    :DbTableCol(name, REAL)
{}

RString RealTabCol::description() const
{
    RString desp = DbTableCol::description();
    if(!default_)
        return desp;
    return RString::FormatString("%s %s %f", desp.cstr(), DEFAULT, defaultvalue_);
}

RealTabCol& RealTabCol::setDefaultValue(double val)
{
    defaultvalue_ = val;
    return *this;
}

RATEL_NAMESPACE_END


