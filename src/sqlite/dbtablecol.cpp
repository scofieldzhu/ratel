/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.sqlite
Module: dbtablecol.cpp
CreateTime: 2019-1-13 20:37
=========================================================================*/
#include "dbtablecol.h"

RATEL_NAMESPACE_BEGIN

DbTableCol::DbTableCol(const RString& name, DataType dt) 
    :name_(name),
    datatype_(dt)
{}

DbTableCol::~DbTableCol()
{}

RString DbTableCol::description() const
{
    RString desp = name()+" "+dataTypeString();
    if(primarykey_){
        desp += " ";
        desp += PRIMARYKEY;
    }
    if(unique_){
        desp += " ";
        desp += UNIQUE;
    }
    if(notnull_){
        desp += " ";
        desp += NOTNULL;
    }
    return desp;
}

DbTableCol& DbTableCol::setPrimaryKey(bool b)
{
    primarykey_ = b;
    return *this;
}

DbTableCol& DbTableCol::primaryKeyOn()
{
    primarykey_ = true;
    return *this;
}

DbTableCol& DbTableCol::setNotNull(bool b)
{
    notnull_ = b;
    return *this;
}

DbTableCol& DbTableCol::notNullOn()
{
    notnull_ = true;
    return *this;
}

DbTableCol& DbTableCol::setUnique(bool b)
{
    unique_ = b;
    return *this;
}

DbTableCol& DbTableCol::uniqueOn()
{
    unique_ = true;
    return *this;
}

DbTableCol& DbTableCol::setDefault(bool b)
{
    default_ = b;
    return *this;
}

DbTableCol& DbTableCol::defaultOn()
{
    default_ = true;
    return *this;
}

RATEL_NAMESPACE_END
