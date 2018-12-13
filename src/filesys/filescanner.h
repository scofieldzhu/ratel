/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: filescanner.h 
CreateTime: 2018-12-13 20:44
=========================================================================*/
#ifndef __filescanner_h__
#define __filescanner_h__

#include "path.h"

RATEL_NAMESPACE_BEGIN
class RATEL_FILESYS_API FileScanner
{
public:
    using PathSet = std::vector<Path>;
    const PathSet& scan(const Path& dir, const PathFilter* f);
    FileScanner() = default;
    ~FileScanner() = default;

private:
    void handleMeet(const Path& target);
    PathSet resultpaths_;
};
RATEL_NAMESPACE_END

#endif
