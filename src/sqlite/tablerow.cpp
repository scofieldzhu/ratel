/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: tablerow.cpp 
CreateTime: 2018-7-25 21:32
=========================================================================*/
#include "tablerow.h"

RATEL_NAMESPACE_BEGIN
TableRow::TableRow(sqlite3_stmt * st)
    :stmt_(st)
{
}

TableRow::~TableRow()
{
}

RString TableRow::queryText(int32 col)
{
    const int32 bytenum = sqlite3_column_bytes(stmt_, col);
    const unsigned char* textbuf = sqlite3_column_text(stmt_, col);
    char* content = new char[bytenum + 1];
    memcpy(content, textbuf, bytenum);
    content[bytenum] = '\0';
    RString restext(content);
    delete[]content;
    return restext;
}

int32 TableRow::queryInteger(int32 col)
{
    return sqlite3_column_int(stmt_, col);
}

double TableRow::queryReal(int32 col)
{
    return sqlite3_column_double(stmt_, col);
}

RATEL_NAMESPACE_END
