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

#include "pkgdb.h"
#include "path.h"

RATEL_NAMESPACE_BEGIN
class DataBlockStorage;
class RATEL_PACKAGE_API Package 
{
public:
    bool createDir(const RString& name, const Path& location);
    bool importDir(const Path& targetdir, const Path& localdir);
    bool removeDir(const Path& dir);
    bool exportDir(const Path& sourcedir, const Path& localdir);
    bool importFile(const Path& dirlocation, const Path& sourcefile);
    bool removeFile(const Path& filepath);
    bool exportFile(const Path& sourcefilepath, const Path& localfile);
    bool load(const Path& pkgpath);
    bool createNew(const Path& newpkgpath);
    void commit();
    bool opened()const;
    void close(bool commitmodified = false);
    const Path& workDir()const { return workdir_; }
    Package(const Path& workdir);
    ~Package();

private:
	bool createRootDir();
	bool doRemoveDir(int32_t dirid);
	bool doImportFile(DataBlockStorage& dbs, int32_t dirid, const Path& sourcefile);
	bool exportDirId(int32_t dirid, DataBlockStorage& dbs, const Path& localdir);
    Path newDBFilePath()const;
    Path newDataStorageFilePath()const;
    Path workdir_;
    Path pkgfile_;    
    PKGDB* pkgdb_ = nullptr;    
};

RATEL_NAMESPACE_END

#endif
