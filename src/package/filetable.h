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
	static const RString kIdKey;
	static const RString kNameKey;
	static const RString kDirIdKey;
	static const RString kFileUIDKey;
	static const RString kStatusKey;	
	bool queryFile(const RString& filename, int32_t dirid, RowDataDict& resultdata);
	bool queryFilesOfDir(int32_t dirid, std::vector<RowDataDict>& resultrows, const RowDataDict& reference);
	int32_t queryFileId(const RString& filename, int32_t dirid);
	bool existsFile(const RString& filename, int32_t dirid);
	bool removeFile(int32_t id);
    FileTable();
    ~FileTable();
};

RATEL_NAMESPACE_END

#endif
