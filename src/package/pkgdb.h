/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: pkgdb.h 
CreateTime: 2019-3-6 21:30
=========================================================================*/
#ifndef __pkgdb_h__
#define __pkgdb_h__

#include "packagepublic.h"
#include "db.h"
#include "dirtable.h"
#include "filetable.h"

RATEL_NAMESPACE_BEGIN
class Path;
class RATEL_PACKAGE_API PKGDB : public DB
{
public:
	DirTable& dirTable() { return dirtable_; }
	FileTable& fileTable() { return filetable_; }
	int32_t queryFileId(const RString& filename, const RString& parentdir);
	bool queryFile(const Path& absfilepath, RowDataDict& resultdata);	
	PKGDB(const Path& dbfile, int32_t flags, const char* zvfs = nullptr);
	~PKGDB();

private:
	DirTable dirtable_;
	FileTable filetable_;
};
RATEL_NAMESPACE_END


#endif
