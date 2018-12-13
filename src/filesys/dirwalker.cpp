/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: dirwalker.cpp 
CreateTime: 2018-12-9 20:18
=========================================================================*/
#include "dirwalker.h"
#include <windows.h>
#include "pathfilter.h"

RATEL_NAMESPACE_BEGIN

DirWalker::DirWalker(const Path& dir, const PathFilter* f)
    :curdir_(dir),
    curfilter_(f)
{}

void DirWalker::walk(MeetFunc& func)
{
    if(!curdir_.exists())
        return;
    Path sortpath = curdir_.join("*.*");
    std::wstring rootdir;
    sortpath.rstring().decodeToWString(rootdir);
    WIN32_FIND_DATA wfd;
    HANDLE fh = ::FindFirstFile(rootdir.c_str(), &wfd);
    if(fh == INVALID_HANDLE_VALUE)
        return;
    do{
        Path newfile = wfd.cFileName;
        Path newfilepath = curdir_.join(newfile);
        if(curfilter_ == nullptr || curfilter_->filter(newfile))
            func(newfilepath);
    }while(::FindNextFile(fh, &wfd));
    ::FindClose(fh);    
}

RATEL_NAMESPACE_END