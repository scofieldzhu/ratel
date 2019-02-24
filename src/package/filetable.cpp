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
#include "sqldatameta.h"
#include "statement.h"
#include "sqlite3.h"
#include "pkglogger.h"

RATEL_NAMESPACE_BEGIN
using namespace sqlkw;

const RString FileTable::kIdKey("id");
const RString FileTable::kNameKey("name");
const RString FileTable::kDirIdKey("dir");
const RString FileTable::kFileUIDKey("fileuid");
const RString FileTable::kStatusKey("status");

FileTable::FileTable()
    :DbTable("File")
{
	PropDict& colpropdict = addNewColumn(kIdKey).setDataMeta(IntSqlDataMeta()).propDict();
	colpropdict.setPropStatusOn(kPrimaryKey).setPropStatusOn(kUnique).setPropStatusOn(kNotNull);
	addNewColumn(kNameKey).setDataMeta(StrSqlDataMeta(100)).propDict().setPropStatusOn(kNotNull);
	addNewColumn(kDirIdKey).setDataMeta(IntSqlDataMeta()).propDict().setPropStatusOn(kNotNull);
	addNewColumn(kFileUIDKey).setDataMeta(IntSqlDataMeta()).propDict().setPropStatusOn(kNotNull);
	addNewColumn(kStatusKey).setDataMeta(IntSqlDataMeta(NORMAL)).propDict().setPropStatusOn(kDefault);    
}

FileTable::~FileTable()
{}

int32_t FileTable::queryId(const RString& filename, int32_t dirid)
{
	if(db_ == nullptr){
		slog_err(pkglogger) << "no any db instance identified!" << endl;
		return -1;
	}
	RString sql = makeQueryRowWhenSql("%s='%s' and %s=%d", kNameKey.cstr(), filename.cstr(), kDirIdKey.cstr(), dirid);
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
