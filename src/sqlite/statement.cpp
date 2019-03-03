/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: statement.cpp 
CreateTime: 2018-11-7 21:05
=========================================================================*/
#include "statement.h"
#include "db.h"
#include "rowdatadict.h"
#include "sqlite3.h"
#include "sqlitelogger.h"

RATEL_NAMESPACE_BEGIN

Statement::Statement(void* handle, DB* db)
    :stmthandle_(handle),
    dbinst_(db)
{
    logverify(sqlitelogger, stmthandle_ && dbinst_);
}

Statement::~Statement()
{
    if(sqlite3_finalize((sqlite3_stmt*)stmthandle_) != SQLITE_OK)
        slog_err(sqlitelogger) << "sqlite3_finalize failed!" << endl;    
}

int32_t Statement::stepExec()
{
    return sqlite3_step((sqlite3_stmt*)stmthandle_);
}

int32_t Statement::fetchColumnCount()
{
    return sqlite3_data_count((sqlite3_stmt*)stmthandle_);
}

int32_t Statement::fetchIntColumnData(int32_t col)
{
    return sqlite3_column_int((sqlite3_stmt*)stmthandle_, col);
}

RString Statement::fetchTextColumnData(int32_t col)
{
    const int32_t bytenum = sqlite3_column_bytes((sqlite3_stmt*)stmthandle_, col);
    const unsigned char* textbuf = sqlite3_column_text((sqlite3_stmt*)stmthandle_, col);
    char* content = new char[bytenum + 1];
    memcpy(content, textbuf, bytenum);
    content[bytenum] = '\0';
    RString restext(content);
    delete[]content;
    return restext;
}

double Statement::fetchDoubleColumnData(int32_t col)
{
    return sqlite3_column_double((sqlite3_stmt*)stmthandle_, col);
}

void Statement::fetchColumnData(const RString& columnname, Variant& resultdata)
{
	sqlite3_stmt* sthandle = static_cast<sqlite3_stmt*>(stmthandle_);
	for(auto i = 0; i < sqlite3_data_count(sthandle); ++i){
		if(columnname == sqlite3_column_name(sthandle, i)){
			fetchColumnData(i, resultdata);
			return;
		}
	}
	resultdata.setNull();
}

void Statement::fetchColumnData(int32_t column, Variant& data)
{
	sqlite3_stmt* sthandle = static_cast<sqlite3_stmt*>(stmthandle_);
	switch(data.dataType()){
		case Variant::kIntType:
			data.setInt32(sqlite3_column_int(sthandle, column));
			break;
		case Variant::kDoubleType:
			data.setDouble(sqlite3_column_double(sthandle, column));
			break;
		case Variant::kStringType:
			data.setStr(fetchTextColumnData(column));
			break;
		default:
			slog_err(sqlitelogger) << "unsupported data type[" << data.dataType() << "]!" << endl;
			break;
	}
}

void Statement::fetchDataDict(RowDataDict& dd)
{
	sqlite3_stmt* sthandle = static_cast<sqlite3_stmt*>(stmthandle_);
	for(auto i = 0; i < sqlite3_data_count(sthandle); ++i){
		RString colname = sqlite3_column_name(sthandle, i);
		if(dd.existKey(colname))
			fetchColumnData(i, dd[colname]);
		if(dd.isIntegrity())
			break;
	}
}

int32_t Statement::reset()
{
    return sqlite3_reset((sqlite3_stmt*)stmthandle_);
}

RString Statement::errMsg()
{
    return sqlite3_errmsg((sqlite3*)(dbinst_->dbconn_));
}

RATEL_NAMESPACE_END