/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: dirnode.h 
CreateTime: 2018-6-6 20:37
=========================================================================*/
#ifndef __dirnode_h__
#define __dirnode_h__

#include "filenode.h"

RATEL_NAMESPACE_BEGIN

struct DirNode
{
    RString dirname;
    FileNodeList allfiles;
    DirNode* parent = nullptr;
    DirNode* next_sibling = nullptr;
    DirNode* next_child = nullptr; 
    DirNode(const RString& name)
        :dirname(name){}
};

RATEL_NAMESPACE_END

#endif
