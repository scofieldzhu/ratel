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
	int32_t queryId(const RString& filename, int32_t dirid);
    FileTable();
    ~FileTable();
};

RATEL_NAMESPACE_END

#endif
