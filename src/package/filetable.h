/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: filetable.h 
CreateTime: 2019-1-13 20:28
=========================================================================*/
#ifndef __filetable_h__
#define __filetable_h__

#include "packagepublic.h"
#include "dbtable.h"

RATEL_NAMESPACE_BEGIN

class RATEL_PACKAGE_API FileTable : public DbTable
{
public:
    FileTable();
    ~FileTable();
};

RATEL_NAMESPACE_END

#endif
