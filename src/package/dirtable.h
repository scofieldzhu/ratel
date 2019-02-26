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
	static const RString kIdKey;
	static const RString kPathKey;
	static const RString kParentKey;
	static const RString kStatusKey;
	int32_t queryDirId(const RString& path);
	int32_t queryDirId(const RString& dirname, int32_t parentdirid);
    DirTable();
    ~DirTable();
};

RATEL_NAMESPACE_END

#endif
