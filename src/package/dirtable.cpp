/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: dirtable.cpp 
CreateTime: 2019-1-13 20:19
=========================================================================*/
#include "dirtable.h"
#include "db.h"
#include "dbtablecol.h"
#include "sqldatameta.h"
#include "statement.h"
#include "sqlite3.h"
#include "pkglogger.h"

RATEL_NAMESPACE_BEGIN
using namespace sqlkw;
const RString DirTable::kIdKey = "id";
const RString DirTable::kPathKey = "path";
const RString DirTable::kParentKey = "parent";
const RString DirTable::kStatusKey = "status";

DirTable::DirTable()
    :DbTable("Directory")
{
	PropDict& colpropdict = addNewColumn(kIdKey).setDataMeta(IntSqlDataMeta()).propDict();
	colpropdict.setPropStatusOn(kPrimaryKey).setPropStatusOn(kUnique).setPropStatusOn(kNotNull);
	addNewColumn(kPathKey).setDataMeta(StrSqlDataMeta(50)).propDict().setPropStatusOn(kNotNull);
    addNewColumn(kParentKey).setDataMeta(IntSqlDataMeta()).propDict().setPropStatusOn(kNotNull);
	addNewColumn(kStatusKey).setDataMeta(IntSqlDataMeta(NORMAL)).propDict().setPropStatusOn(kDefault);    
}

DirTable::~DirTable()
{}

int32_t DirTable::queryDirId(const RString& path)
{	
	RString sql = makeQueryRowWhenSql("%s='%s'", kPathKey.cstr(), path.cstr());
	return queryPrimaryKeyId(sql);
}

int32_t DirTable::queryDirId(const RString& dirname, int32_t parentdirid)
{
	RString sql = makeQueryRowWhenSql("%s LIKE '%%/%s' and %s=%d", kPathKey.cstr(), dirname.cstr(), kParentKey.cstr(), parentdirid);
	return queryPrimaryKeyId(sql);
}

RATEL_NAMESPACE_END