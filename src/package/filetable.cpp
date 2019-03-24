/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: filetable.cpp 
CreateTime: 2019-1-13 20:28
=========================================================================*/
#include "filetable.h"
#include "db.h"
#include "dbtablecol.h"
#include "rowdatadict.h"
#include "sqldatameta.h"
#include "statement.h"
#include "sqlite3.h"
#include "path.h"
#include "pkglogger.h"

RATEL_NAMESPACE_BEGIN
using namespace sqlkw;

const RString FileTable::kIdKey("file_id");
const RString FileTable::kNameKey("file_name");
const RString FileTable::kDirIdKey("file_dir");
const RString FileTable::kFileUIDKey("file_fileuid");
const RString FileTable::kStatusKey("file_status");

FileTable::FileTable()
    :DbTable("File")
{
	PropDict& colpropdict = addNewColumn(kIdKey).setDataMeta(IntSqlDataMeta()).propDict();
	colpropdict.setPropStatusOn(kPrimaryKey).setPropStatusOn(kUnique).setPropStatusOn(kNotNull);
	addNewColumn(kNameKey).setDataMeta(StrSqlDataMeta(100)).propDict().setPropStatusOn(kNotNull);
	addNewColumn(kDirIdKey).setDataMeta(IntSqlDataMeta()).propDict().setPropStatusOn(kNotNull);
	addNewColumn(kFileUIDKey).setDataMeta(StrSqlDataMeta(32)).propDict().setPropStatusOn(kNotNull);
	addNewColumn(kStatusKey).setDataMeta(IntSqlDataMeta(NORMAL)).propDict().setPropStatusOn(kDefault);    
}

FileTable::~FileTable()
{}

int32_t FileTable::queryFileId(const RString& filename, int32_t dirid)
{	
	RString sql = makeQueryRowWhenSql("%s='%s' and %s=%d", kNameKey.cstr(), filename.cstr(), kDirIdKey.cstr(), dirid);	
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance connected!" << endl;
		return -1;
	}
	Variant data(Variant::kIntType);
	return db_->queryColumnValueOfFirstResultRow(sql, 0, data) ? data.convertToInt32() : -1;
}

bool FileTable::queryFile(const RString& filename, int32_t dirid, RowDataDict& resultdata)
{	
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance connected!" << endl;
		return false;
	}
	RString sql = makeQueryRowWhenSql("%s='%s' and %s=%d", kNameKey.cstr(), filename.cstr(), kDirIdKey.cstr(), dirid);
	return db_->queryFirstResultRowData(sql, resultdata);
}

bool FileTable::queryFilesOfDir(int32_t dirid, std::vector<RowDataDict>& resultrows, const RowDataDict& reference)
{
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance connected!" << endl;
		return false;
	}
	RString sql = makeQueryRowWhenSql("%s=%d", kDirIdKey.cstr(), dirid);	
	return db_->queryMultiResultRowData(sql, resultrows, reference);
}

bool FileTable::existsFile(const RString& filename, int32_t dirid) 
{
	return queryFileId(filename, dirid) != -1;
}

bool FileTable::removeFile(int32_t id)
{
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance connected!" << endl;
		return false;
	}
	return db_->execUpdateData(makeDelRowWhenSql("%s=%d", kIdKey, id));
}

RATEL_NAMESPACE_END
