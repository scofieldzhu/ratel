/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: filetable.cpp 
CreateTime: 2019-1-13 20:28
=========================================================================*/
#include "filetable.h"
#include "inttabcol.h"
#include "strtabcol.h"

RATEL_NAMESPACE_BEGIN

namespace{
    const char* kIdField = "id";
    const char* kNameField = "name";
    const char* kDirField = "dir";
    const char* kStatusField = "status";
}

FileTable::FileTable()
    :DbTable("File")
{
    addColumn(IntTabCol(kIdField).primaryKeyOn().uniqueOn().notNullOn());
    addColumn(StrTabCol(kNameField, 100).notNullOn());
    addColumn(IntTabCol(kDirField).setDefaultValue(-1).defaultOn());
    addColumn(IntTabCol(kStatusField).setDefaultValue(1).defaultOn());
}

FileTable::~FileTable()
{}

RATEL_NAMESPACE_END
