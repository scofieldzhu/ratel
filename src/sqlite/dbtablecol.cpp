/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.sqlite
Module: dbtablecol.cpp
CreateTime: 2019-1-13 20:37
=========================================================================*/
#include "dbtablecol.h"
#include "sqldatameta.h"

RATEL_NAMESPACE_BEGIN
using namespace sqlkw;

DbTableCol::DbTableCol(const RString& name)
	:name_(name),
	colpropdict_({kPrimaryKey, kNotNull, kUnique, kDefault})
{}

RString DbTableCol::createSql()const
{
	if(sqldatameta_ == nullptr)
		return "";
	RString sql = name_ + " " + sqldatameta_->sql();
	if(colpropdict_.getPropStatus(sqlkw::kPrimaryKey))
		sql = sql + " " + sqlkw::kPrimaryKey;
	if(colpropdict_.getPropStatus(sqlkw::kUnique))
		sql = sql + " " + sqlkw::kUnique;
	if(colpropdict_.getPropStatus(sqlkw::kNotNull))
		sql = sql + " " + sqlkw::kNotNull;
	if(colpropdict_.getPropStatus(sqlkw::kDefault))
		sql = sql + " " + sqldatameta_->defaultValue();
	return sql;
}

RATEL_NAMESPACE_END