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

void DirWalker::walk(MeetFunc func)
{
    if(!curdir_.exists())
        return;
    Path sortpath = curdir_.join("*.*");
    std::wstring rootdir;
    sortpath.rstring().decodeToWString(rootdir);
    WIN32_FIND_DATA wfd;
    memset(std::addressof(wfd.cFileName), 0, sizeof(wfd.cFileName[0]) * MAX_PATH);
    HANDLE fh = ::FindFirstFile(rootdir.c_str(), &wfd);
    if(fh == INVALID_HANDLE_VALUE)
        return;
    do{
        const std::wstring newfilename = wfd.cFileName;
        if(newfilename == L"." || newfilename == L".."){
            memset(std::addressof(wfd.cFileName), 0, sizeof(wfd.cFileName[0]) * MAX_PATH);
            continue;
        }
        Path newfilepath = curdir_.join(newfilename.c_str());
        func(newfilepath);
        memset(std::addressof(wfd.cFileName), 0, sizeof(wfd.cFileName[0]) * MAX_PATH);
    }while(::FindNextFile(fh, &wfd));
    ::FindClose(fh);    
}

RATEL_NAMESPACE_END