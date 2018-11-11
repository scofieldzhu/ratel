/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: dirtree.cpp 
=========================================================================*/
#include "dirtree.h"
#include <algorithm>
#include "dirnode.h"
#include "pkglogger.h"
#include "path.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

namespace {

    void SplitFilePath(const RString& source_filepath, RString& dir, RString& filename)
    {
        if(source_filepath.empty())
            return;
        size_t pos = source_filepath.findLastOf(PATH_SEPARATOR);
        if(pos != RString::npos){
            dir = source_filepath.substr(0, pos);
            filename = source_filepath.substr(pos + 1);
        }else 
            filename = source_filepath;
    }
}

DirTree::DirTree()
    :rootnode_(new DirNode(ROOTDIR_NAME))
{}

DirTree::~DirTree()
{
    deleteDirNode(rootnode_);
}

DirNode* DirTree::createDir(const RString& name, const Path& parentpath)
{
    DirNode* parentnode = locateDir(parentpath);
    if(parentnode == nullptr){
        slog_warn(pkglogger) << "parent path[" << parentpath.cstr() << "] is invalid!" << endl;
        return nullptr;
    }
    //check if exists same dirname in children
    if(parentnode->findChild(name)){
        slog_warn(pkglogger) << "dir already exists with name[" << name.cstr() << "]!" << endl;
        return nullptr;
    }
    DirNode* newdir = new DirNode(name);
    newdir->parent = parentnode;
    //append child
    DirNode* prechild = findLastChildDir(parentnode);
    if(!prechild)
        parentnode->nextchild = newdir;
    else 
        prechild->nextsibling = newdir;    
    return newdir;
}

DirNode* DirTree::locateDir(const Path& dirpath)
{
    if(dirpath.empty())
        return nullptr;
    RStrings splitdirstrs = dirpath.rstring().split(PATH_SEPARATOR);
    DirNode* targetnode = nullptr;
    DirNode* curnode = rootnode_;
    for(int32 i = 0; i < splitdirstrs.size(); ++i){
        const RString kCurDirName = splitdirstrs[i];
        while(curnode){
            if(curnode->name == kCurDirName)
                break;
            curnode = curnode->nextsibling;
        }
        if(curnode == nullptr)
            return nullptr;
        targetnode = curnode;
        curnode = curnode->nextchild;
    }
    return targetnode;
}

// DirNode* DirTree::locateDir(const Path& dirpath) 
// {
//     const DirNode* thenode = const_cast<const DirTree*>(this)->locateDir(dirpath);
//     return const_cast<DirNode*>(thenode);
// }

bool DirTree::renameDir(const RString& dirpath, const RString& newname)
{
    DirNode* dirnode = locateDir(dirpath);
    if(!dirnode){
        slog_warn(pkglogger) << "dir[" << dirpath.cstr() << "] not exists" << endl;
        return false;
    }
    DirNode* parent_dirnode = dirnode->parent;
    if(!parent_dirnode){ //modify name directly
        dirnode->name = newname;
        return true;
    }
    const DirNode* exists_node = parent_dirnode->findChild(newname);
    if(exists_node && exists_node != dirnode){
        slog_warn(pkglogger) << "sibling dir [" << newname.cstr() << "] already exists" << endl;
        return false;
    }
    dirnode->name = newname;
    return true;    
}

bool DirTree::createFile(const RString& filepath)
{
    RString dirpath, filename;
    SplitFilePath(filepath, dirpath, filename);
    if(dirpath.empty() || filename.empty())
        return false;
    DirNode* dirnode = locateDir(dirpath);
    if(!dirnode){
        slog_warn(pkglogger) << "dir[" << dirpath.cstr() << "] not exists" << endl;
        return false;
    }
    if(dirnode->findFile(filename) == -1){ //file not exists at present
        FileNode newnode;
        newnode.filename = filename;
        newnode.diskfileid = 0;
        dirnode->allfiles.push_back(newnode);
        return true;
    }
    return false;    
}

bool DirTree::renameFile(const RString& filepath, const RString& newfilename)
{
    RString dirpath, filename;
    SplitFilePath(filepath, dirpath, filename);
    if(dirpath.empty() || filename.empty())
        return false;
    DirNode* dirnode = locateDir(dirpath);
    if(!dirnode || !dirnode->existFile(filename)){
        slog_warn(pkglogger) << "the file[" << filepath.cstr() << "] not exists" << endl;
        return false;
    }
    if(dirnode->existFile(newfilename)){
        slog_warn(pkglogger) << "cannot rename to newfilename[" << newfilename.cstr() << "] already existed!" << endl;
        return false;
    }
    int32 fidx = dirnode->findFile(filename);
    dirnode->allfiles[fidx].filename = newfilename;    
    return true;    
}

void DirTree::deleteFile(const RString& filepath)
{
    RString dirpath, filename;
    SplitFilePath(filepath, dirpath, filename);
    if(dirpath.empty())
        return;
    DirNode* dirnode = locateDir(dirpath);
    if(!dirnode){
        slog_warn(pkglogger) << "dir[" << dirpath.cstr() << "] not exists" << endl;
        return;
    }
    int32 fileidx = dirnode->findFile(filename);
    if(fileidx)
        dirnode->allfiles.erase(dirnode->allfiles.begin() + fileidx);
}

bool DirTree::findFile(const RString& filepath, FileNode& outfn)
{
    RString dirpath, filename;
    SplitFilePath(filepath, dirpath, filename);
    if(dirpath.empty() || filename.empty())
        return false;
    DirNode* dirnode = locateDir(dirpath);
    if(!dirnode)
        return false;
    int32 fileidx = dirnode->findFile(filename);
    if(fileidx){
        outfn = dirnode->allfiles[fileidx];
        return true;
    }
    return false;    
}

void DirTree::deleteDir(const RString& dirpath)
{
    if(dirpath == ROOTDIR_NAME){
        slog_warn(pkglogger) << "root dir[" << dirpath.cstr() << "] cannot be deleted!" << endl;
        return;
    }
    DirNode* thedirnode = locateDir(dirpath);
    if(thedirnode == nullptr){
        slog_warn(pkglogger) << "dir[" << dirpath.cstr() << "] not exists" << endl;
        return;
    }
    deleteDirNode(thedirnode);
}

void DirTree::deleteDirNode(DirNode* dirnode)
{
    logverify(pkglogger, dirnode != nullptr);
    detachChildNode(*dirnode->parent, dirnode->name);
    if(!dirnode->nextchild){
        delete dirnode; // release this node's memory
        return;
    }
    DirNode* nextsibling = nullptr;
    for(DirNode* curchild = dirnode->nextchild; curchild != nullptr; curchild = nextsibling){
        nextsibling = curchild->nextsibling;
        deleteDirNode(curchild);
    }        
}

void DirTree::detachChildNode(DirNode& parent, const RString& childname)
{
    DirNode* cur = parent.nextchild;
    DirNode* pre = nullptr;
    while(cur){
        if(cur->name != childname){
            pre = cur;
            cur = cur->nextsibling;
            continue;
        }
        if(pre) 
            pre->nextsibling = cur->nextsibling;                
        else                
            parent.nextchild = cur->nextsibling;
        cur->nextsibling = nullptr;
        cur->parent = nullptr;
        break;
    }
}

DirNode* DirTree::findLastChildDir(DirNode* parent) const
{
    DirNode* curchild = parent->nextchild;
    if(!curchild)
        return nullptr;
    DirNode* lastnode = nullptr;
    while(curchild){
        lastnode = curchild;
        curchild = curchild->nextsibling;
    }
    return lastnode;
}

RATEL_NAMESPACE_END

