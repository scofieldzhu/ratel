/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: sqlrecord.cpp 
CreateTime: 2019-7-30 21:05
=======================================================================*/
#include "sqlrecord.h"

RATEL_NAMESPACE_BEGIN

SqlRecord::SqlRecord()
{}

SqlRecord::~SqlRecord()
{}

Variant SqlRecord::value(int32_t pos) const
{
    Variant v;
    if(pos >= 0 && pos < fields_.size())
        v = fields_[pos].value();
    return v;
}

Variant SqlRecord::value(const RString& name) const
{
    Variant v;
    auto it = std::find_if(fields_.begin(), fields_.end(), [&name](const SqlField& f){ return f.name() == name; });
    if(it != fields_.end())
        v = it->value();
    return v;
}

void SqlRecord::setValue(int32_t pos, const Variant& v)
{
    if(pos >= 0 && pos < fields_.size())
        fields_[pos].setValue(v);
}

void SqlRecord::setValue(const RString& name, const Variant& v)
{
    auto it = std::find_if(fields_.begin(), fields_.end(), [&name](const SqlField& f){ return f.name() == name; });
    if(it != fields_.end())
        it->setValue(v);
}

int32_t SqlRecord::indexOf(const RString& nm) const
{
    for(int32_t i = 0; i < fields_.size(); ++i){
        if(fields_[i].name() == nm)
            return i;
    }
    return -1;
}

RString SqlRecord::fieldName(int32_t pos) const
{
    if(pos >= 0 && pos < fields_.size())
        return fields_[pos].name();
    return RString();
}

SqlField SqlRecord::field(int32_t pos) const
{
    if(pos >= 0 && pos < fields_.size())
        return fields_[pos];
    return SqlField();
}

SqlField SqlRecord::field(const RString& name) const
{
    auto it = std::find_if(fields_.begin(), fields_.end(), [&name](const SqlField& f){ return f.name() == name; });
    if(it != fields_.end())
        return *it;
    return {};
}

bool SqlRecord::exists(const SqlField& field) const
{
    return std::find(fields_.begin(), fields_.end(), field) != fields_.end();
}

void SqlRecord::append(const SqlField& field)
{
    fields_.push_back(field);
}

void SqlRecord::replace(int32_t pos, const SqlField& field)
{
    if(pos >= 0 && pos < fields_.size())
        fields_[pos] = field;
}

void SqlRecord::insert(int32_t pos, const SqlField& field)
{
    if(pos >= 0 && pos < fields_.size())
        fields_.insert(fields_.begin() + pos, field); 
}

void SqlRecord::remove(int32_t pos)
{
    if(pos >= 0 && pos < fields_.size())
        fields_.erase(fields_.begin() + pos);
}

bool SqlRecord::empty() const
{
    return fields_.empty();
}

std::size_t SqlRecord::count() const
{
    return fields_.size();
}

RATEL_NAMESPACE_END