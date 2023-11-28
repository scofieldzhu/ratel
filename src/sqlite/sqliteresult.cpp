/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqliteresult.cpp 
CreateTime: 2019-8-27 21:11
=======================================================================*/
#include "sqliteresult.h"
#include "sqlite3.h"
#include "sqlitecon.h"
#include "sqlitelogger.h"

RATEL_NAMESPACE_BEGIN

namespace
{
    SqlDataType SqliteTypeToSqlDataType(int32_t sqltype) 
    {
        static const std::map<int32_t, SqlDataType> stDataTypeMap = 
        {
            {SQLITE_INTEGER, SqlDataType::kInteger},
            {SQLITE_FLOAT, SqlDataType::kReal},
            {SQLITE_BLOB, SqlDataType::kBinary},
            {SQLITE_TEXT, SqlDataType::kText},
            {SQLITE_NULL, SqlDataType::kNull},
        };
        return stDataTypeMap.at(sqltype);
    }
}

SqliteResult::SqliteResult(SqliteCon& con)
    :conn_(con)
{}

SqliteResult::~SqliteResult()
{
    finalize();
}

bool SqliteResult::fetchNext()
{
    if(stmt_){
        auto retcode = sqlite3_step((sqlite3_stmt*)stmt_);
        if(retcode == SQLITE_ROW){
            updateRecord();
            return true;
        }
        if(retcode != SQLITE_DONE)
            slog_warn(sqlitelogger) << "sqlite3_prepare_v2 error! code=" << retcode << std::endl;        
    }    
    return false;
}

void SqliteResult::updateRecord()
{
    auto* sthandle = static_cast<sqlite3_stmt*>(stmt_);
    for(auto col = 0; col < sqlite3_data_count(sthandle); ++col){
        SqlDataType coltype = SqliteTypeToSqlDataType(sqlite3_column_type(sthandle, col));
        SqlField newfield(sqlite3_column_name(sthandle, col), coltype);
        switch(coltype){
            case SqlDataType::kNull:
            {
                //default value is null.
                break;
            }                
            case SqlDataType::kInteger:
            {
                int32_t v = sqlite3_column_int(sthandle, col);
                newfield.setValue(v);
                break;
            }                
            case SqlDataType::kReal:
            {
                double v = sqlite3_column_double(sthandle, col);
                newfield.setValue(v);
                break;
            }                
            case SqlDataType::kText:
            {
                const int32_t bytenum = sqlite3_column_bytes(sthandle, col);
                const unsigned char* textbuf = sqlite3_column_text(sthandle, col);
                char* content = new char[bytenum + 1];
                memcpy(content, textbuf, bytenum);
                content[bytenum] = '\0';
                newfield.setValue(content);
                break;
            }                
            case SqlDataType::kBinary:
            {
                //cannot handle binary data at present!
//                 const int32_t bytenum = sqlite3_column_bytes(sthandle, col);
//                 sqlite3_column_blob()
                break;
            }                
            default:
                break;
        }
        currecord_.append(newfield);
    }
}

int32_t SqliteResult::currentColumnCount()
{
    if(stmt_)
        return sqlite3_data_count((sqlite3_stmt*)stmt_);
    return 0;
}

bool SqliteResult::exec(const RString& sql)
{
    Ignore(sql);
    return false; //not support at present!
}

bool SqliteResult::prepare(const RString& sql)
{
    if(!conn_.isOpened())
        return false;
    sqlite3_stmt* resstmt = nullptr;
    auto* db = static_cast<sqlite3*>(conn_.handle());
    auto err = sqlite3_prepare_v2(db, sql.cstr(), (int)sql.size(), &resstmt, nullptr);
    if(err != SQLITE_OK){
        slog_err(sqlitelogger) << "sqlite3_prepare_v2 error:" << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    stmt_ = resstmt;
    return true;
}

bool SqliteResult::finalize()
{
    if(stmt_){
        auto errcode = sqlite3_finalize((sqlite3_stmt*)stmt_);
        if(errcode != SQLITE_OK){
            slog_err(sqlitelogger) << "sqlite3_finalize failed and errcode=" << errcode <<"!" << endl;
            return false;
        }
        stmt_ = nullptr;
    }
    return true;
}

RATEL_NAMESPACE_END