/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: dirwalker.h 
CreateTime: 2018-12-9 20:18
=========================================================================*/
#ifndef __dirwalker_h__
#define __dirwalker_h__

#include "path.h"

RATEL_NAMESPACE_BEGIN

class RATEL_FILESYS_API DirWalker
{
public:
    typedef std::function<void(const Path& path)> MeetFunc;
    void walk(MeetFunc& func);
    void setFilter(const PathFilter* f) { curfilter_ = f; }
    const PathFilter* filter()const { return curfilter_; }
    void setDir(const Path& dir) { curdir_ = dir; }
    const Path& dir()const { return curdir_; }
    DirWalker(const Path& dir, const PathFilter* f);
    ~DirWalker() = default;

private:
    Path curdir_;
    const PathFilter* curfilter_;
};

RATEL_NAMESPACE_END

#endif
