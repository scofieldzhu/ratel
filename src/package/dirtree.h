/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: dirtree.h 
=========================================================================*/
#ifndef __dirtree_h__
#define __dirtree_h__

#include "basic.h"
#include "packagepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_PACKAGE_API DirTree
{
public:
    DirNode* createDir(const RString& dirname, const RString& parent_path);
    DirNode* findDir(const RString& dirpath)const;
    DirTree();
    ~DirTree();

private:   
    DirNode* findLastChildDir(DirNode* parent)const;
    DirNode* rootnode_;
};
RATEL_NAMESPACE_END
#endif
