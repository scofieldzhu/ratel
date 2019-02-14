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

DbTableRecord::DbTableRecord(const vector<RString>& colkeys)
{
	for(auto k : colkeys)
		colvals_.insert({k, Variant()});
}

DbTableRecord::DbTableRecord(const map<RString, Variant>& colvals)
	:colvals_(colvals)
{}

DbTableRecord::DbTableRecord(const DbTableRecord& rhs)
	:colvals_(rhs.colvals_)
{}

DbTableRecord& DbTableRecord::operator=(const DbTableRecord& rhs)
{
	colvals_ = rhs.colvals_;
	return *this;
}

bool DbTableRecord::existColumn(const RString& key) const
{
	return colvals_.find(key) != colvals_.end();
}

void DbTableRecord::setColumnValue(const RString& key, const Variant& result)
{
	auto it = colvals_.find(key);
	if(it != colvals_.end())
		it->second = result;	
}

Variant DbTableRecord::getColumnValue(const RString& key) const
{
	auto it = colvals_.find(key);
	return it == colvals_.end() ? Variant() : it->second;	
}

Variant& DbTableRecord::operator[](const RString& key)
{
	auto it = colvals_.find(key);
	logverify(sqlitelogger, it != colvals_.end());
	return it->second;
}

const Variant& DbTableRecord::operator[](const RString& key) const
{
	auto it = colvals_.find(key);
	logverify(sqlitelogger, it != colvals_.end());
	return it->second;
}

vector<RString> DbTableRecord::allColumns() const
{
	vector<RString> columns;
	for(auto it = colvals_.begin(); it != colvals_.end(); ++it)
		columns.push_back(it->first);
	return columns;
}

RATEL_NAMESPACE_END


