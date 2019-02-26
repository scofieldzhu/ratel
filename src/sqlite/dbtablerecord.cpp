/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: dbtablerow.cpp 
CreateTime: 2019-1-26 19:08
=========================================================================*/
#include "dbtablerecord.h"
#include "dbtablecol.h"
#include "sqlitelogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

DbTableRecord::DbTableRecord()
{}

DbTableRecord::DbTableRecord(const map<RString, Variant>& colvals)
	:columnvalues_(colvals)
{}

DbTableRecord::DbTableRecord(const DbTableRecord& rhs)
	:columnvalues_(rhs.columnvalues_)
{}

DbTableRecord::~DbTableRecord()
{}

DbTableRecord& DbTableRecord::operator=(const DbTableRecord& rhs)
{
	columnvalues_ = rhs.columnvalues_;
	return *this;
}

bool DbTableRecord::existColumn(const RString& key) const
{
	return columnvalues_.find(key) != columnvalues_.end();
}

DbTableRecord& DbTableRecord::addColumn(const RString& key, const Variant& value)
{
	if(existColumn(key))
		setColumnValue(key, value);
	else 
		columnvalues_.insert({key, value});
	return *this;
}

DbTableRecord& DbTableRecord::setColumnValue(const RString& key, const Variant& result)
{
	auto it = columnvalues_.find(key);
	if(it != columnvalues_.end())
		it->second = result;	
	return *this;
}

Variant DbTableRecord::getColumnValue(const RString& key) const
{
	auto it = columnvalues_.find(key);
	return it == columnvalues_.end() ? Variant() : it->second;	
}

Variant& DbTableRecord::operator[](const RString& key)
{
	auto it = columnvalues_.find(key);
	logverify(sqlitelogger, it != columnvalues_.end());
	return it->second;
}

const Variant& DbTableRecord::operator[](const RString& key) const
{
	auto it = columnvalues_.find(key);
	logverify(sqlitelogger, it != columnvalues_.end());
	return it->second;
}

vector<RString> DbTableRecord::allColumns() const
{
	vector<RString> columns;
	for(auto it = columnvalues_.begin(); it != columnvalues_.end(); ++it)
		columns.push_back(it->first);
	return columns;
}

void DbTableRecord::reset()
{
	columnvalues_.clear();
}

RATEL_NAMESPACE_END


