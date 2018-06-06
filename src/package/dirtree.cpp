/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: dirtree.cpp 
=========================================================================*/
#include "dirtree.h"
#include "dirnode.h"
#include "packagelogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

DirTree::DirTree()
    :rootnode_(new DirNode(ROOTDIR_NAME))
{}

DirTree::~DirTree()
{}

DirNode* DirTree::createDir(const RString& dirname, const RString& parent_path)
{
    DirNode* parent = findDir(parent_path);
    if(parent == nullptr){
        slog_warn(packagelogger) << "parent path[" << parent_path.cstr() << "] is invalid!" << endl;
        return nullptr;
    }
    //check if exists same dirname in children
    DirNode* curchild = parent->next_child;
    while(curchild){
        if(curchild->dirname == dirname){
            slog_warn(packagelogger) << "dir already exists with name[" << dirname.cstr() << "]!" << endl;
            return nullptr;
        }
        curchild = curchild->next_sibling;
    }
    DirNode* newdir = new DirNode(dirname);
    newdir->parent = parent;
    //append child
    DirNode* prechild = findLastChildDir(parent);
    if (!prechild)
        parent->next_child = newdir;
    else 
        prechild->next_sibling = newdir;    
    return newdir;
}

DirNode* DirTree::findDir(const RString& dirpath) const
{
    if(dirpath.empty())
        return nullptr;
    RStrings path_strs = dirpath.split(PATH_SEPARATOR);
    DirNode* targetnode = nullptr;
    DirNode* curnode = rootnode_;
    for(int32 i = 0; i < path_strs.size(); ++i){
        const RString curdir = path_strs[i];
        bool matched = false;
        while(curnode){
            if (curnode->dirname == curdir){
                matched = true;
                break;
            }
            curnode = curnode->next_sibling;
        }
        if(!matched)
            return nullptr;
        targetnode = curnode;
        curnode = curnode->next_child;
    }
    return targetnode;
}

DirNode* DirTree::findLastChildDir(DirNode* parent) const
{
    DirNode* curchild = parent->next_child;
    if(!curchild)
        return nullptr;
    DirNode* lastnode = nullptr;
    while(curchild){
        lastnode = curchild;
        curchild = curchild->next_sibling;
    }
    return lastnode;
}

RATEL_NAMESPACE_END

