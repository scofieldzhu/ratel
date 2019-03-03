/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: dbtablerow.cpp 
CreateTime: 2019-1-26 19:08
=========================================================================*/
#include "rowdatadict.h"
#include "sqlitelogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

RowDataDict::RowDataDict()
{}

RowDataDict::RowDataDict(const map<RString, Variant>& values)
	:datadict_(values)
{}

RowDataDict::RowDataDict(const RowDataDict& rhs)
	:datadict_(rhs.datadict_)
{}

RowDataDict::~RowDataDict()
{}

RowDataDict& RowDataDict::operator=(const RowDataDict& rhs)
{
	datadict_ = rhs.datadict_;
	return *this;
}

bool RowDataDict::existKey(const RString& key) const
{
	return datadict_.find(key) != datadict_.end();
}

RowDataDict& RowDataDict::insertPair(const RString& key, const Variant& value)
{
	if(existKey(key))
		setValue(key, value);
	else 
		datadict_.insert({key, value});
	return *this;
}

RowDataDict& RowDataDict::setValue(const RString& key, const Variant& result)
{
	auto it = datadict_.find(key);
	if(it != datadict_.end())
		it->second = result;	
	return *this;
}

RowDataDict& RowDataDict::setValue(const RString& key, int32_t value)
{
	return setValue(key, Variant(value));
}

RowDataDict& RowDataDict::setValue(const RString& key, double value)
{
	return setValue(key, Variant(value));
}

RowDataDict& RowDataDict::setValue(const RString& key, const RString& value)
{
	return setValue(key, Variant(value.cstr()));
}

Variant RowDataDict::getValue(const RString& key) const
{
	auto it = datadict_.find(key);
	return it == datadict_.end() ? Variant() : it->second;	
}

Variant& RowDataDict::operator[](const RString& key)
{
	auto it = datadict_.find(key);
	logverify(sqlitelogger, it != datadict_.end());
	return it->second;
}

const Variant& RowDataDict::operator[](const RString& key) const
{
	auto it = datadict_.find(key);
	logverify(sqlitelogger, it != datadict_.end());
	return it->second;
}

vector<RString> RowDataDict::keys() const
{
	vector<RString> columns;
	for(auto it = datadict_.begin(); it != datadict_.end(); ++it)
		columns.push_back(it->first);
	return columns;
}

void RowDataDict::reset()
{
	datadict_.clear();
}

bool RowDataDict::isIntegrity() const
{
	for(auto it = datadict_.begin(); it != datadict_.end(); ++it){
		if(it->second.isNull())
			return false;
	}
	return true;
}

RATEL_NAMESPACE_END


