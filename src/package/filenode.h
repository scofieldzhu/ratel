/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: filenode.h 
=========================================================================*/
#ifndef __filenode_h__
#define __filenode_h__

#include "packagepublic.h"

RATEL_NAMESPACE_BEGIN

struct FileNode
{
    RString filename;
    int32_t datafileid = 0;
    int32_t dbid = -1;
    int32_t init = 0;
};
using FileNodeList = std::vector<FileNode>;

RATEL_NAMESPACE_END
#endif
