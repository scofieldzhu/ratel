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

class RATEL_SQLITE_API RowDataDict
{
public:
	void reset();
	bool existKey(const RString& key)const;
	RowDataDict& insertPair(const RString& key, const Variant& value);	
	RowDataDict& remove(const RString& key);
	RowDataDict& setValue(const RString& key, const Variant& value);
	RowDataDict& setValue(const RString& key, int32_t value);
	RowDataDict& setValue(const RString& key, double value);
	RowDataDict& setValue(const RString& key, const RString& value);
	Variant getValue(const RString& key) const;
	Variant& operator[](const RString& key);
	const Variant& operator[](const RString& key)const;
	std::vector<RString> keys()const;
	std::size_t keyCount()const { return datadict_.size(); }
	bool isIntegrity()const;
	void combine(const RowDataDict& other);
	RowDataDict();
	RowDataDict(const std::map<RString, Variant>&);
	RowDataDict(const RowDataDict&);
	RowDataDict& operator=(const RowDataDict&);
	~RowDataDict();

private:
	std::map<RString, Variant> datadict_;
};

RATEL_NAMESPACE_END

#endif
