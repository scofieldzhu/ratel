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
    const DirNode* rootNode()const { return rootnode_; }
    DirNode* createDir(const RString& name, const Path& location);
    const DirNode* locateDir(const Path& dirpath)const;
    DirNode* locateDir(const Path& dir);
    bool renameDir(const RString& dirpath, const RString& newname);
    void deleteDir(const RString& dirpath);
    FileNode* createFile(const Path& dir, const RString& filename);
    bool renameFile(const RString& filepath, const RString& newfilename);
    void deleteFile(const RString& filepath);
    bool findFile(const RString& filepath, FileNode& outfn);    
    DirTree();
    ~DirTree();

private:       
    void deleteDirNode(DirNode* dirnode);
    void detachChildNode(DirNode& parent, const RString& childname);
    DirNode* findLastChildDir(DirNode* parent)const;
    DirNode* rootnode_;
};
RATEL_NAMESPACE_END
#endif
