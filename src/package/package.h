/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: package.h 
CreateTime: 2018-9-9 21:41
=========================================================================*/
#ifndef __package_h__
#define __package_h__

#include "packagepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_PACKAGE_API Package 
{
public:
    const RString& lastError()const;
    bool createDir(const RString& name, const Path& location);
    bool importDir(const Path& targetdir, const Path& localdir);
    bool removeDir(const Path& dir);
    bool exportDir(const Path& sourcedir, const Path& local_targetdir);
    bool importFile(const Path& dirlocation, const Path& sourcefile);
    bool removeFile(const Path& filepath);
    bool exportFile(const Path& sourcefilepath, const Path& local_targetfilepath);
    bool load(const Path& pkgpath);
    bool createNew(const Path& newpkgpath);
    bool opened()const;
    Package();
    ~Package();
private:
    struct Impl;
    Impl* impl_;
};

RATEL_NAMESPACE_END

#endif
