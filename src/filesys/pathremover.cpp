/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: pathremover.cpp 
CreateTime: 2018-12-23 21:46
=========================================================================*/
#include "pathremover.h"
#include <windows.h>
#include "fsyslogger.h"
#include "dirwalker.h"
using std::vector;

RATEL_NAMESPACE_BEGIN

PathRemover::PathRemover()
{}

PathRemover::~PathRemover()
{}

void PathRemover::perform(const Path& srcpath, bool remsubifdir)
{
    if(srcpath.isRegularFile()){
        std::wstring wstrpath = srcpath.toWString();
        if(!::DeleteFile(wstrpath.c_str()))
            slog_err(fsyslogger) << "Delete File[" << srcpath.cstr() << endl;
        return;
    }
    removeDir(srcpath, remsubifdir);
}

void PathRemover::removeDir(const Path& sourcedir, bool remsubifdir)
{    
    vector<Path> subdirs;
    auto f = [&subdirs](const Path& p){
        if(p.isRegularFile()){
            std::wstring wstrpath = p.toWString();
            if(!::DeleteFile(wstrpath.c_str()))
                slog_err(fsyslogger) << "Delete File[" << p.cstr() << "] Failed! errcode=" << ::GetLastError() << "." << endl;
        }else subdirs.push_back(p);
    };
    DirWalker(sourcedir).walk(f);
    if(!remsubifdir)
        return;
    for(const auto& dir : subdirs)
        removeDir(dir, remsubifdir);
    if(!::RemoveDirectory(sourcedir.toWString().c_str())){
        slog_err(fsyslogger) << "Delete Empty Directory[" << sourcedir.cstr() << "] Failed! errcode=" << ::GetLastError() << "." << endl;
        return;
    }
}

RATEL_NAMESPACE_END
