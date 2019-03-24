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
	bool queryDir(const RString& path, RowDataDict& resultdata);
	int32_t queryDirId(const RString& path);
	int32_t queryDirId(const RString& dirname, int32_t parentdirid);
	bool querySubDirIds(int32_t parentid, std::vector<int32_t>& subids);
	bool querySubDirs(int32_t dirid, std::vector<RowDataDict>& resultrows, const RowDataDict& reference);
    DirTable();
    ~DirTable();
};

RATEL_NAMESPACE_END

#endif
