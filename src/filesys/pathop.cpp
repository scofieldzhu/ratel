/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: pathop.cpp 
CreateTime: 2019-3-20 21:18
=========================================================================*/
#include "pathop.h"

RATEL_NAMESPACE_BEGIN

bool CreateDir(const Path& dirpath)
{
	return ::CreateDirectory(dirpath.toWString().c_str(), nullptr);
}

RATEL_NAMESPACE_END
