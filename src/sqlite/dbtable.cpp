/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: table.cpp 
CreateTime: 2018-7-28 12:07
=========================================================================*/
#include "dbtable.h"
#include "db.h"
#include "dbtablecol.h"
#include "rowdatadict.h"
#include "sqldatameta.h"
#include "sqlitelogger.h"
#include "sqlite3.h"
#include "statement.h"

RATEL_NAMESPACE_BEGIN
DbTable::DbTable(const RString & name, DB* db)
    :name_(name),
	db_(db)
{}

DbTable::~DbTable()
{
	for(auto col : columns_)
		rtdelete(col);
}

bool DbTable::checkTableRecordValidity(const RowDataDict& record) const
{
	for(auto k : record.keys()){
		if(!existsColumn(k))
			return false;
	}
	return true;
}

DbTableCol& DbTable::addNewColumn(const RString& key)
{
	DbTableCol* existingcolumn = getColumn(key);
	if(existingcolumn)
		return *existingcolumn;
	existingcolumn = new DbTableCol(key);
	columns_.push_back(existingcolumn);
	return *existingcolumn;
}

DbTable& DbTable::removeColumn(const RString & colname)
{
    auto it = std::find_if(columns_.begin(), columns_.end(), [&colname](const DbTableCol* tc) {return colname == tc->name();});
    if(it != columns_.end())
        columns_.erase(it);    
    return *this;
}

DbTableCol* DbTable::getColumn(const RString & colname)const
{
    auto it = std::find_if(columns_.begin(), columns_.end(), [&colname](const DbTableCol* tc) {return colname == tc->name();});
    return it == columns_.end() ? nullptr : *it;    
}

DbTableCol * DbTable::getColumn(int32_t colidx)const
{
    return (colidx < 0 && colidx >= columns_.size()) ? nullptr : columns_[colidx];    
}

RString DbTable::makeCreateSql(bool ifexists)
{
    RString sql = "create table ";
    if(ifexists)
        sql += "if not exists ";
    sql += (name_ + "(");
    for(const DbTableCol* col : columns_)
        sql += (col->createSql() + ",");
    sql.back() = ')';
    return sql;
}

RString DbTable::makeInsertRowSql(const RowDataDict& record)
{
	logverify(sqlitelogger, checkTableRecordValidity(record) && record.keyCount() > 0);
	RString ressql = "insert into " + name_ + "(";
	for(auto colkey : record.keys())
		ressql += (colkey + ",");
	ressql.back() = ')';
	ressql += " values (";
	for(auto colkey : record.keys()){
		const DbTableCol* thiscol = getColumn(colkey);
		switch(thiscol->dataMeta()->dataType()){
			case SqlDataMeta::kStr:
				ressql += RString::FormatString("'%s',", record[colkey].convertToStr().cstr());
				break;
			case SqlDataMeta::kInt:
				ressql += (RString::FromInt32(record[colkey].convertToInt32()) + ",");
				break; 
			case SqlDataMeta::kReal:
				ressql += (RString::FromDouble(record[colkey].convertToDouble()) + ",");
				break;
		}		
	}
	ressql.back() = ')';
	ressql += ";";
	return ressql;
}

RString DbTable::makeDelRowWhenSql(const char* whenfmt, ...)
{
    static const int32_t kMaxBufSize = 400;
    char buffer[kMaxBufSize] = {'\0'};
    va_list vl;
    va_start(vl, whenfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, whenfmt, vl);
    va_end(vl);
    return "DELETE FROM " + name_ + " WHERE " + buffer + ";";
}

RString DbTable::makeQueryRowWhenSql(const char* whenfmt, ...)
{
    static const int32_t kMaxBufSize = 400;
    char buffer[kMaxBufSize] = { '\0' };
    va_list vl;
    va_start(vl, whenfmt);
    vsprintf_s(buffer, kMaxBufSize - 1, whenfmt, vl);
    va_end(vl);
    return "select * from " + name_ + " where " + buffer + ";";
}

RString DbTable::makeDropSql()
{
    return "drop table " + name_ + ";";
}

bool DbTable::create()
{
	if(db_ == nullptr){
		slog_err(sqlitelogger) << "no any db instance identified!" << endl;
		return false;
	}
	RString createsql = makeCreateSql();
	Statement* stat = db_->createStatement(createsql);
	if(stat == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << createsql.cstr() << " err:" << db_->errMsg().cstr() << endl;
		return false;
	}
	if(stat->stepExec() != SQLITE_DONE){		
		slog_err(sqlitelogger) << "stepExec failed! sql:" << createsql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		delete stat;
		return false;
	}
	delete stat;
	return true;
}

bool DbTable::insertRow(const RowDataDict& record)
{
	if(db_ == nullptr){
		slog_err(sqlitelogger) << "no any db instance identified!" << endl;
		return false;
	}
	RString sql = makeInsertRowSql(record);
	Statement* stat = db_->createStatement(sql);
	if(stat == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << db_->errMsg().cstr() << endl;
		return false;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_DONE){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		delete stat;
		return false;
	}
	return true;	
}

Variant DbTable::queryColumnValueOfFirstResultRow(const RString& sql, const RString& columnkey)
{	
	Variant resultval;
	if(db_ == nullptr){
		slog_err(sqlitelogger) << "no any db instance identified!" << endl;
		return resultval;
	}
	const DbTableCol* column = getColumn(columnkey);
	if(column == nullptr){
		slog_err(sqlitelogger) << "invalid column key[" << columnkey.cstr() << "] identified!" << endl;
		return resultval;
	}
	switch(column->dataMeta()->dataType()){
		case SqlDataMeta::kInt:
			resultval.setDataType(Variant::kIntType);
			break;
		case SqlDataMeta::kReal:
			resultval.setDataType(Variant::kDoubleType);
			break;
		case SqlDataMeta::kStr:
			resultval.setDataType(Variant::kStringType);
			break;
	}		
	Statement* stat = db_->createStatement(sql);
	if(stat == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << db_->errMsg().cstr() << endl;
		return resultval;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_ROW){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		delete stat;
		return resultval;
	}
	stat->fetchColumnData(columnkey, resultval);
	delete stat;
	return resultval;
}

void DbTable::drop()
{
	if(db_ == nullptr){
		slog_err(sqlitelogger) << "no any db instance identified!" << endl;
		return;
	}
	RString sql = makeDropSql();
	Statement* stat = db_->createStatement(sql);
	if(stat == nullptr){
		slog_err(sqlitelogger) << "create statement failed! sql:" << sql.cstr() << " err:" << db_->errMsg().cstr() << endl;
		return;
	}
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_DONE)
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;		
	delete stat;
}

void DbTable::connectDB(DB& db)
{
	db_ = &db;
	RString sql = makeCreateSql();
	Statement* stat = db_->createStatement(sql);
	int32_t rc = stat->stepExec();
	if(rc != SQLITE_DONE){
		slog_err(sqlitelogger) << "stepExec failed! sql:" << sql.cstr() << " err:" << stat->errMsg().cstr() << endl;
		db_ = nullptr;
	}
}

bool DbTable::queryTableExistence()
{
	RString sql = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
	Statement* stat = db_->createStatement(sql);
	logverify(sqlitelogger, stat != nullptr);	
	int32_t rc = stat->stepExec();
	while(rc == SQLITE_ROW){
		if(stat->fetchTextColumnData(0) == name_)
			return true;
		rc = stat->stepExec();
	}
	return false;
}

void DbTable::disconnectDB()
{

}

RATEL_NAMESPACE_END


