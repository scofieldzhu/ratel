/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: fsutil.h 
CreateTime: 2019-3-17 20:32
=========================================================================*/
#ifndef __fsutil_h__
#define __fsutil_h__

#include "kernelpublic.h"

RATEL_NAMESPACE_BEGIN

namespace fsutil{
	RATEL_KERNEL_API bool TransformDataBlock(std::ifstream& srcfs, std::ofstream& dstfs, uint32_t totalbytesize, uint32_t buffersize);
	RATEL_KERNEL_API bool TransformDataBlock(HANDLE srcfh, HANDLE dstfh, uint32_t totalbytesize, uint32_t buffersize);
}

RATEL_NAMESPACE_END

#endif
