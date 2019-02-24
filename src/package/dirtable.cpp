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

int32_t DirTable::queryId(const RString& path)
{
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance identified!" << endl;
		return -1;
	}
	RString sql = makeQueryRowWhenSql("%s='%s'", kPathKey.cstr(), path.cstr());
	Statement* stat = db_->createStatement(sql);
	if(stat == nullptr){
		slog_err(pkglogger) << "create statement failed! sql:" << sql.cstr() << " err:" << db_->errMsg().cstr() << endl;
		return -1;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_ROW){
		slog_err(pkglogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		delete stat;
		return -1;
	}
	int32_t rid = stat->fetchIntColumn(0);
	delete stat;
	return rid;
}

RATEL_NAMESPACE_END