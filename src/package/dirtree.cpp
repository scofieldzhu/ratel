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
#include "packagelogger.h"
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
{}

const DirNode* DirTree::createDir(const RString& dirname, const RString& parent_path)
{
    DirNode* parent = findDir(parent_path);
    if(parent == nullptr){
        slog_warn(packagelogger) << "parent path[" << parent_path.cstr() << "] is invalid!" << endl;
        return nullptr;
    }
    //check if exists same dirname in children
    if(parent->findChildDir(dirname)){
        slog_warn(packagelogger) << "dir already exists with name[" << dirname.cstr() << "]!" << endl;
        return nullptr;
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

const DirNode* DirTree::findDir(const RString& dirpath) const
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
            if(curnode->dirname == curdir){
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

DirNode* DirTree::findDir(const RString& dirpath)
{
    return const_cast<DirNode*>(const_cast<const DirTree*>(this)->findDir(dirpath));
}

bool DirTree::renameDir(const RString& dirpath, const RString& newname)
{
    DirNode* dirnode = findDir(dirpath);
    if(!dirnode){
        slog_warn(packagelogger) << "dir[" << dirpath.cstr() << "] not exists" << endl;
        return false;
    }
    DirNode* parent_dirnode = dirnode->parent;
    if(!parent_dirnode){ //modify name directly
        dirnode->dirname = newname;
        return true;
    }
    const DirNode* exists_node = parent_dirnode->findChildDir(newname);
    if(exists_node && exists_node != dirnode){
        slog_warn(packagelogger) << "sibling dir [" << newname.cstr() << "] already exists" << endl;
        return false;
    }
    dirnode->dirname = newname;
    return true;    
}

bool DirTree::createFile(const RString& filepath)
{
    RString dirpath, filename;
    SplitFilePath(filepath, dirpath, filename);
    if(dirpath.empty() || filename.empty())
        return false;
    DirNode* dirnode = findDir(dirpath);
    if(!dirnode){
        slog_warn(packagelogger) << "dir[" << dirpath.cstr() << "] not exists" << endl;
        return false;
    }
    if(dirnode->findFile(filename) == -1){ //file not exists at present
        FileNode new_filenode;
        new_filenode.filename = filename;
        new_filenode.datafile_id = 0;
        dirnode->allfiles.push_back(new_filenode);
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
    DirNode* dirnode = findDir(dirpath);
    if(!dirnode || !dirnode->existFile(filename)){
        slog_warn(packagelogger) << "the file[" << filepath.cstr() << "] not exists" << endl;
        return false;
    }
    if(dirnode->existFile(newfilename)){
        slog_warn(packagelogger) << "cannot rename to newfilename[" << newfilename.cstr() << "] already existed!" << endl;
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
    DirNode* dirnode = findDir(dirpath);
    if(!dirnode){
        slog_warn(packagelogger) << "dir[" << dirpath.cstr() << "] not exists" << endl;
        return;
    }
    int32 fileidx = dirnode->findFile(filename);
    if(fileidx)
        dirnode->allfiles.erase(dirnode->allfiles.begin() + fileidx);
}

bool DirTree::findFile(const RString& filepath, FileNode& outfn)const
{
    RString dirpath, filename;
    SplitFilePath(filepath, dirpath, filename);
    if(dirpath.empty() || filename.empty())
        return false;
    const DirNode* dirnode = findDir(dirpath);
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
    DirNode* thedirnode = findDir(dirpath);
    if(thedirnode == nullptr){
        slog_warn(packagelogger) << "dir[" << dirpath.cstr() << "] not exists" << endl;
        return;
    }
    deleteDirNode(thedirnode);
}

void DirTree::deleteDirNode(DirNode* dirnode)
{
    logverify(packagelogger, dirnode != nullptr);
    detachChildNode(*dirnode->parent, dirnode->dirname);
    if(!dirnode->next_child){
        delete dirnode; // release this node's memory
        return;
    }
    DirNode* nextsibling = nullptr;
    for(DirNode* curchild = dirnode->next_child; curchild != nullptr; curchild = nextsibling){
        nextsibling = curchild->next_sibling;
        deleteDirNode(curchild);
    }        
}

void DirTree::detachChildNode(DirNode& parent, const RString& childname)
{
    DirNode* cur = parent.next_child;
    DirNode* pre = nullptr;
    while(cur){
        if(cur->dirname != childname){
            pre = cur;
            cur = cur->next_sibling;
            continue;
        }
        if(pre) 
            pre->next_sibling = cur->next_sibling;                
        else                
            parent.next_child = cur->next_sibling;
        cur->next_sibling = nullptr;
        cur->parent = nullptr;
        break;
    }
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

