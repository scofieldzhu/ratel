/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: dirnode.cpp 
CreateTime: 2018-6-10 21:45
=========================================================================*/
#include "dirnode.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

DirNode::DirNode(const RString& namestr)
    :name(namestr)
{}

bool DirNode::existFile(const RString& filename) const
{
    return findFile(filename) >= 0;
}

int32_t DirNode::findFile(const RString& filename)const
{
    auto it = find_if(allfiles.begin(), allfiles.end(), [&filename](const FileNode& fn){return fn.filename == filename;});
    if(it == allfiles.end())
        return -1;
    return it - allfiles.begin();
}

const DirNode* DirNode::findSibling(const RString& siblingname)
{
    if(parent == nullptr)
        return nullptr;
    const DirNode* curchild = parent->nextchild;
    while(curchild){
        if(curchild == this)
            continue;
        if (curchild->name == name)
            return curchild;
        curchild = curchild->nextsibling;
    }
    return nullptr;

}

const DirNode* DirNode::findChild(const RString& dirname) const
{
    const DirNode* curchild = nextchild;
    while(curchild){
        if(curchild->name == dirname)
            return curchild;
        curchild = curchild->nextsibling;
    }
    return nullptr;
}

RString DirNode::path() const
{
    RString pathstr;
    const DirNode* p = this;
    while(p){
        pathstr = p->name + PATH_SEPARATOR + pathstr;
        p = p->parent;
    }
    return pathstr;
}

RATEL_NAMESPACE_END
