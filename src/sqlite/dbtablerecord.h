/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: dbtablerecord.h 
CreateTime: 2019-1-26 18:49
=========================================================================*/
#ifndef __dbtablerecord_h__
#define __dbtablerecord_h__

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API DbTableRecord
{
public:
	bool existColumn(const RString& key)const;
	void setColumnValue(const RString& key, const Variant& result);
	Variant getColumnValue(const RString& key) const;
	Variant& operator[](const RString& key);
	const Variant& operator[](const RString& key)const;
	std::vector<RString> allColumns()const;
	std::size_t columnCount()const { return colvals_.size(); }
	DbTableRecord(const std::vector<RString>& colkeys);
	DbTableRecord(const std::map<RString, Variant>& colvals);
	DbTableRecord(const DbTableRecord&);
	DbTableRecord& operator=(const DbTableRecord&);
	~DbTableRecord();

private:
	std::map<RString, Variant> colvals_;
};

RATEL_NAMESPACE_END

#endif
