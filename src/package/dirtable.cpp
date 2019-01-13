/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: dirtable.cpp 
CreateTime: 2019-1-13 20:19
=========================================================================*/
#include "dirtable.h"
#include "dbtablecol.h"
#include "inttabcol.h"
#include "strtabcol.h"

RATEL_NAMESPACE_BEGIN

namespace{
    const char* kIdField = "id";
    const char* kNameField = "name";
    const char* kParentField = "parent";
    const char* kStatusField = "status";
}

DirTable::DirTable()
    :DbTable("Directory")
{
    addColumn(IntTabCol(kIdField).primaryKeyOn().uniqueOn().notNullOn());
    addColumn(StrTabCol(kNameField, 50).notNullOn());
    addColumn(IntTabCol(kParentField).setDefaultValue(-1).defaultOn());
    addColumn(IntTabCol(kStatusField).setDefaultValue(1).defaultOn());
}

DirTable::~DirTable()
{}

RATEL_NAMESPACE_END