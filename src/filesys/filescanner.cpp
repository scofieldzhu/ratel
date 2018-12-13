/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: filescanner.cpp 
CreateTime: 2018-12-13 20:49
=========================================================================*/
#include "filescanner.h"
#include "dirwalker.h"

RATEL_NAMESPACE_BEGIN

const FileScanner::PathSet& FileScanner::scan(const Path& dir, const PathFilter* f)
{
    resultpaths_.clear();
    DirWalker dw(dir, f);
    DirWalker::MeetFunc mf = std::bind(&FileScanner::handleMeet, this, std::placeholders::_1);
    dw.walk(mf);
    return resultpaths_;
}

void FileScanner::handleMeet(const Path& target)
{
    if(target.isRegularFile())
        resultpaths_.push_back(target);
}

RATEL_NAMESPACE_END
