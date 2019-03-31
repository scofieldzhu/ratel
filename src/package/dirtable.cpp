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
#include "rowdatadict.h"
#include "sqldatameta.h"
#include "statement.h"
#include "pkglogger.h"

RATEL_NAMESPACE_BEGIN
using namespace sqlkw;
const RString DirTable::kIdKey = "dir_id";
const RString DirTable::kPathKey = "dir_path";
const RString DirTable::kParentKey = "dir_parent";
const RString DirTable::kStatusKey = "dir_status";

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
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance connected!" << endl;
		return -1;
	}
	RString sql = makeQueryRowWhenSql("%s='%s'", kPathKey.cstr(), path.cstr());
	Variant data(Variant::kIntType);
	return db_->queryColumnValueOfFirstResultRow(sql, 0, data) ? data.convertToInt32() : -1;
}

bool DirTable::querySubDirIds(int32_t parentid, std::vector<int32_t>& subids)
{	
	RowDataDict reference({{kIdKey, Variant(Variant::kIntType)}});
	std::vector<RowDataDict> resultrows;
	if(querySubDirs(parentid, resultrows, reference)){
		for(auto row : resultrows)
			subids.push_back(row[kIdKey].convertToInt32());
		return true;
	}
	return false;
}

bool DirTable::querySubDirs(int32_t parentid, std::vector<RowDataDict>& resultrows, const RowDataDict& reference)
{
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance connected!" << endl;
		return false;
	}
	RString sql = makeQueryRowWhenSql("%s=%d", kParentKey.cstr(), parentid);
	return db_->queryMultiResultRowData(sql, resultrows, reference);	
}

bool DirTable::queryDir(const RString& path, RowDataDict& resultdata)
{	
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance connected!" << endl;
		return false;
	}
	RString sql = makeQueryRowWhenSql("%s='%s'", kPathKey.cstr(), path.cstr());
	return db_->queryFirstResultRowData(sql, resultdata);
}

int32_t DirTable::queryDirId(const RString& dirname, int32_t parentdirid)
{
	RString sql = makeQueryRowWhenSql("%s LIKE '%%/%s' and %s=%d", kPathKey.cstr(), dirname.cstr(), kParentKey.cstr(), parentdirid);
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance connected!" << endl;
		return -1;
	}
	Variant data(Variant::kIntType);
	return db_->queryColumnValueOfFirstResultRow(sql, 0, data) ? data.convertToInt32() : -1;
}

RATEL_NAMESPACE_END