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
    isautovalue_(rhs.isautovalue_),
    isreadonly_(rhs.isreadonly_)
{}

void SqlField::setValue(const Variant& v)
{
    if(!isreadonly_)
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
    isautovalue_ = rhs.isautovalue_;
    isreadonly_ = rhs.isreadonly_;
    return *this;
}

bool SqlField::operator==(const SqlField& rhs)
{
    bool state = (name_ == rhs.name_ && value_ == rhs.value_);
    state = state && (datatype_ == rhs.datatype_);
    state = state && (defaultvalue_ == rhs.defaultvalue_);
    state = state && (length_ == rhs.length_);
    state = state && (precision_ == rhs.precision_);
    state = state && (isautovalue_ == rhs.isautovalue_);
    state = state && (isreadonly_ == rhs.isreadonly_);
    return state;
}

bool SqlField::operator!=(const SqlField& rhs)
{
    return !operator==(rhs);
}

RATEL_NAMESPACE_END
