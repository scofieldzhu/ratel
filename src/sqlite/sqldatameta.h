/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqldata.h 
CreateTime: 2019-1-26 20:50
=========================================================================*/
#ifndef __sqldata_h__
#define __sqldata_h__

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

class SqlDataMeta
{
public:
	enum DataType{kInt, kReal, kStr};
	virtual RString sql()const = 0;
	virtual RString defaultValue()const = 0;
	virtual DataType dataType()const = 0;
	virtual SqlDataMeta* clone()const = 0;
	virtual ~SqlDataMeta() = default;
};

class IntSqlDataMeta : public SqlDataMeta
{
public:
	DataType dataType()const { return kInt; }
	RString sql()const { return "INTEGER"; }
	RString defaultValue()const { return RString::FormatString("%d", defvalue_); }
	SqlDataMeta* clone()const{ return new IntSqlDataMeta(*this); }
	IntSqlDataMeta(int32_t defval = 0)
		:defvalue_(defval){}
	IntSqlDataMeta(const IntSqlDataMeta& rhs)
		:defvalue_(rhs.defvalue_){}
	~IntSqlDataMeta() = default;
private:
	int32_t defvalue_;
};

class RealSqlDataMeta : public SqlDataMeta 
{
public:
	DataType dataType()const { return kReal; }
	RString sql()const { return "REAL"; }
	RString defaultValue()const { return RString::FormatString("%lf", defvalue_); }
	SqlDataMeta* clone()const{ return new RealSqlDataMeta(*this); }
	RealSqlDataMeta(double defval = 0.0)
		:defvalue_(defval){}
	RealSqlDataMeta(const RealSqlDataMeta& rhs)
		:defvalue_(rhs.defvalue_){}
	~RealSqlDataMeta() = default;
private:
	double defvalue_;
};

class StrSqlDataMeta : public SqlDataMeta 
{
public:
	DataType dataType()const { return kStr; }
	RString sql()const{ return RString::FormatString("VARCHAR(%d)", maxcharsnum_); };
	RString defaultValue()const{ return RString::FormatString("'%lf'", defvalue_.cstr()); }
	SqlDataMeta* clone()const{ return new StrSqlDataMeta(*this); }
	StrSqlDataMeta(uint32_t maxcharsnum, const char* defval = "")
		:maxcharsnum_(maxcharsnum),
		defvalue_(defval){}
	StrSqlDataMeta(const StrSqlDataMeta& rhs)
		:maxcharsnum_(rhs.maxcharsnum_),
		defvalue_(rhs.defvalue_){}
	~StrSqlDataMeta() = default;
private:
	uint32_t maxcharsnum_;
	RString defvalue_;
};

RATEL_NAMESPACE_END

#endif