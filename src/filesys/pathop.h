/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: pathop.h 
CreateTime: 2019-3-20 21:18
=========================================================================*/
#ifndef __pathop_h__
#define __pathop_h__

#include "path.h"

RATEL_NAMESPACE_BEGIN
RATEL_FILESYS_API bool CreateDir(const Path& dirpath);
RATEL_FILESYS_API bool RemoveDir(const Path& dirpath);
RATEL_FILESYS_API bool RemoveFile(const Path& path);
RATEL_NAMESPACE_END
#endif
