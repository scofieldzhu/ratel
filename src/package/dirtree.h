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
    const DirNode* createDir(const RString& dirname, const RString& parent_path);
    const DirNode* findDir(const RString& dirpath)const;
    bool renameDir(const RString& dirpath, const RString& newname);
    void deleteDir(const RString& dirpath);
    bool createFile(const RString& filepath);
    bool renameFile(const RString& filepath, const RString& newfilename);
    void deleteFile(const RString& filepath);
    bool findFile(const RString& filepath, FileNode& outfn)const;    
    DirTree();
    ~DirTree();

private:   
    DirNode* findDir(const RString& dirpath);
    void deleteDirNode(DirNode* dirnode);
    void detachChildNode(DirNode& parent, const RString& childname);
    DirNode* findLastChildDir(DirNode* parent)const;
    DirNode* rootnode_;
};
RATEL_NAMESPACE_END
#endif
