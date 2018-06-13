/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: filenode.h 
=========================================================================*/
#ifndef __filenode_h__
#define __filenode_h__

#include "basic.h"
#include "packagepublic.h"

RATEL_NAMESPACE_BEGIN

struct FileNode
{
    RString filename;
    int32 datafile_id = 0;
};
using FileNodeList = std::vector<FileNode>;

RATEL_NAMESPACE_END
#endif
