/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: table.cpp 
CreateTime: 2018-7-28 12:07
=========================================================================*/
#include "table.h"
#include <cstdarg>
#include "tablecol.h"
#include "sqlitelogger.h"

RATEL_NAMESPACE_BEGIN
Table::Table(const RString & name)
    :name_(name) 
{}

Table::~Table()
{}

Table& Table::addColumn(const TableCol& col)
{
    auto it = std::find_if(columns_.begin(), columns_.end(), [&col](const TableCol* tc) {return col.name() == tc->name();});
    if(it != columns_.end()){
        slog_warn(sqlitelogger) << "column(" << col.name().cstr() << ") already exists!" << std::endl;
        return *this;
    }
    columns_.push_back(col.clone());
    return *this;
}

Table& Table::removeColumn(const RString & colname)
{
    auto it = std::find_if(columns_.begin(), columns_.end(), [&colname](const TableCol* tc) {return colname == tc->name();});
    if(it != columns_.end())
        columns_.erase(it);    
    return *this;
}

TableCol* Table::getColumn(const RString & colname)
{
    auto it = std::find_if(columns_.begin(), columns_.end(), [&colname](const TableCol* tc) {return colname == tc->name();});
    return it == columns_.end() ? nullptr : *it;    
}

TableCol * Table::getColumn(int32 colidx)
{
    return (colidx < 0 && colidx >= columns_.size()) ? nullptr : columns_[colidx];    
}

RString Table::makeCreateSql(bool ifexists)
{
    RString sql = "create table ";
    if(ifexists)
        sql += "if not exists ";
    sql += (name_ + "(");
    for(const TableCol* col : columns_)
        sql += (col->description() + ",");
    sql.back() = ')';
    return sql;
}

RString Table::makeInsertSql(const char* valfmt, ...)
{
    static const int32 kMaxBufSize = 400;
    char buffer[kMaxBufSize] = { '\0' };
    va_list vl;
    va_start(vl, valfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, valfmt, vl);
    va_end(vl);
    return "insert into " + name_ + " values(" + buffer + ");";
}

RString Table::makeInsertSql(std::initializer_list<const char*> fields, const char* valfmt, ...)
{
    RString ressql = "insert into " + name_ + "(";
    for(auto f : fields)
        ressql += (RString(f) + ",");
    ressql.back() = ')';
    static const int32 kMaxBufSize = 400;
    char buffer[kMaxBufSize] = {'\0'};
    va_list vl;
    va_start(vl, valfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, valfmt, vl);
    va_end(vl);
    return ressql + " values(" + buffer + ");";
}

RString Table::makeDelRowWhenSql(const char* whenfmt, ...)
{
    static const int32 kMaxBufSize = 400;
    char buffer[kMaxBufSize] = { '\0' };
    va_list vl;
    va_start(vl, whenfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, whenfmt, vl);
    va_end(vl);
    return "delete * from " + name_ + " where " + buffer + ";";
}

RString Table::makeQueryRowWhenSql(const char* whenfmt, ...)
{
    static const int32 kMaxBufSize = 400;
    char buffer[kMaxBufSize] = { '\0' };
    va_list vl;
    va_start(vl, whenfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, whenfmt, vl);
    va_end(vl);
    return "select * from " + name_ + " where " + buffer + ";";
}

RString Table::makeDropSql()
{
    return "drop table " + name_ + ";";
}
RATEL_NAMESPACE_END
