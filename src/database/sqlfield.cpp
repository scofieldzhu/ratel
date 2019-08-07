/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: sqlfield.cpp 
CreateTime: 2019-7-30 21:05
=======================================================================*/
#include "sqlfield.h"

RATEL_NAMESPACE_BEGIN

SqlField::SqlField(const RString& name /*= ""*/, SqlDataType t /*= SqlDataType::kInvalid*/)
    :name_(name),
    datatype_(t)
{}

SqlField::SqlField(const SqlField& rhs)
    :name_(rhs.name_),
    value_(rhs.value_),
    datatype_(rhs.datatype_),
    defaultvalue_(rhs.defaultvalue_),
    length_(rhs.length_),
    precision_(rhs.precision_),
    autovalue_(rhs.autovalue_),
    readonly_(rhs.readonly_)
{}

void SqlField::setValue(const Variant& v)
{
    if(!readonly_)
        value_ = v;
}

const SqlField& SqlField::operator=(const SqlField& rhs)
{
    name_ = rhs.name_;
    value_ = rhs.value_;
    datatype_ = rhs.datatype_;
    defaultvalue_ = rhs.defaultvalue_;
    length_ = rhs.length_;
    precision_ = rhs.precision_;
    autovalue_ = rhs.autovalue_;
    readonly_ = rhs.readonly_;
    return *this;
}

bool SqlField::operator==(const SqlField& rhs)const
{    
    return equalWith(rhs);
}

bool SqlField::operator!=(const SqlField& rhs)const
{
    return !operator==(rhs);
}

bool SqlField::equalWith(const SqlField& rhs) const
{
    bool state = (name_ == rhs.name_ && value_ == rhs.value_);
    state = state && (datatype_ == rhs.datatype_);
    state = state && (defaultvalue_ == rhs.defaultvalue_);
    state = state && (length_ == rhs.length_);
    state = state && (precision_ == rhs.precision_);
    state = state && (autovalue_ == rhs.autovalue_);
    state = state && (readonly_ == rhs.readonly_);
    return state;
}

RATEL_NAMESPACE_END
