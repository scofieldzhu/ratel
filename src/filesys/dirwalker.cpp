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

RATEL_NAMESPACE_BEGIN

DirWalker::DirWalker(const Path& dir)
    :curdir_(dir)
{}

void DirWalker::walk(MeetFunc& func)
{
    if(!curdir_.exists())
        return;
    std::wstring rootdir;
    curdir_.rstring().decodeToWString(rootdir);
    WIN32_FIND_DATA wfd;
    HANDLE fh = ::FindFirstFile(rootdir.c_str(), &wfd);
    if(fh == INVALID_HANDLE_VALUE)
        return;
    do{
        func(wfd.cFileName);
    }while(::FindNextFile(fh, &wfd));
    ::FindClose(fh);    
}

RATEL_NAMESPACE_END