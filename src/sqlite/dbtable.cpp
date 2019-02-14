/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: table.cpp 
CreateTime: 2018-7-28 12:07
=========================================================================*/
#include "dbtable.h"
#include <cstdarg>
#include "dbtablecol.h"
#include "dbtablerecord.h"
#include "sqldatameta.h"
#include "sqlitelogger.h"

RATEL_NAMESPACE_BEGIN
DbTable::DbTable(const RString & name)
    :name_(name) 
{}

DbTable::~DbTable()
{}

bool DbTable::checkTableRecordValidity(const DbTableRecord& record) const
{
	for(auto k : record.allColumns()){
		if(!existsColumn(k))
			return false;
	}
	return true;
}

DbTableCol& DbTable::addNewColumn(const RString& key)
{
	DbTableCol* existingcolumn = getColumn(key);
	if(existingcolumn)
		return *existingcolumn;
	existingcolumn = new DbTableCol(key);
	columns_.push_back(existingcolumn);
	return *existingcolumn;
}

DbTable& DbTable::removeColumn(const RString & colname)
{
    auto it = std::find_if(columns_.begin(), columns_.end(), [&colname](const DbTableCol* tc) {return colname == tc->name();});
    if(it != columns_.end())
        columns_.erase(it);    
    return *this;
}

DbTableCol* DbTable::getColumn(const RString & colname)const
{
    auto it = std::find_if(columns_.begin(), columns_.end(), [&colname](const DbTableCol* tc) {return colname == tc->name();});
    return it == columns_.end() ? nullptr : *it;    
}

DbTableCol * DbTable::getColumn(int32_t colidx)const
{
    return (colidx < 0 && colidx >= columns_.size()) ? nullptr : columns_[colidx];    
}

RString DbTable::makeCreateSql(bool ifexists)
{
    RString sql = "create table ";
    if(ifexists)
        sql += "if not exists ";
    sql += (name_ + "(");
    for(const DbTableCol* col : columns_)
        sql += (col->createSql() + ",");
    sql.back() = ')';
    return sql;
}

RString DbTable::makeInsertRowSql(const char* valfmt, ...)
{
    static const int32_t kMaxBufSize = 400;
    char buffer[kMaxBufSize] = { '\0' };
    va_list vl;
    va_start(vl, valfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, valfmt, vl);
    va_end(vl);
    return "insert into " + name_ + " values(" + buffer + ");";
}

RString DbTable::makeInsertRowSql(std::initializer_list<const char*> fields, const char* valfmt, ...)
{
    RString ressql = "insert into " + name_ + "(";
    for(auto f : fields)
        ressql += (RString(f) + ",");
    ressql.back() = ')';
    static const int32_t kMaxBufSize = 400;
    char buffer[kMaxBufSize] = {'\0'};
    va_list vl;
    va_start(vl, valfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, valfmt, vl);
    va_end(vl);
    return ressql + " values(" + buffer + ");";
}

RString DbTable::makeInsertRowSql(const DbTableRecord& record)
{
	logverify(sqlitelogger, checkTableRecordValidity(record) && record.columnCount() > 0);
	RString ressql = "insert into " + name_ + "(";
	for(auto colkey : record.allColumns())
		ressql += (colkey + ",");
	ressql.back() = ')';
	ressql += " values (";
	for(auto colkey : record.allColumns()){
		const DbTableCol* thiscol = getColumn(colkey);
		switch(thiscol->dataMeta()->dataType()){
			case SqlDataMeta::kStr:
				ressql += RString::FormatString("'%s',", record[colkey].convertToStr().cstr());
				break;
			case SqlDataMeta::kInt:
				ressql += (RString::FromInt32(record[colkey].convertToInt32()) + ",");
				break; 
			case SqlDataMeta::kReal:
				ressql += (RString::FromDouble(record[colkey].convertToDouble()) + ",");
				break;
		}		
	}
	ressql.back() = ')';
	ressql += ";";
	return ressql;
}

RString DbTable::makeDelRowWhenSql(const char* whenfmt, ...)
{
    static const int32_t kMaxBufSize = 400;
    char buffer[kMaxBufSize] = { '\0' };
    va_list vl;
    va_start(vl, whenfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, whenfmt, vl);
    va_end(vl);
    return "delete * from " + name_ + " where " + buffer + ";";
}

RString DbTable::makeQueryRowWhenSql(const char* whenfmt, ...)
{
    static const int32_t kMaxBufSize = 400;
    char buffer[kMaxBufSize] = { '\0' };
    va_list vl;
    va_start(vl, whenfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, whenfmt, vl);
    va_end(vl);
    return "select * from " + name_ + " where " + buffer + ";";
}

RString DbTable::makeDropSql()
{
    return "drop table " + name_ + ";";
}
RATEL_NAMESPACE_END
