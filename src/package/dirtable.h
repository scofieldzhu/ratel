/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: dirtable.h 
CreateTime: 2019-1-13 20:19
=========================================================================*/
#ifndef __dirtable_h__
#define __dirtable_h__

#include "packagepublic.h"
#include "dbtable.h"

RATEL_NAMESPACE_BEGIN

class RATEL_PACKAGE_API DirTable : public DbTable
{
public:
    DirTable();
    ~DirTable();
};

RATEL_NAMESPACE_END

#endif
