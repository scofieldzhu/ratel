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
#include "table.h"
#include "path.h"

RATEL_NAMESPACE_BEGIN
class DB;
class DirTree;
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
    void commit();
    bool opened()const;
    void close();
    void setWorkDir(const Path& dir);
    const Path& workDir()const{ return workdir_; }
    Package(const Path& workdir);
    ~Package();

private:
    void releaseResources();
    bool initDB();
    void releaseDB();
    int32 writeNewFileData(const Path& sourcefile);
    int32 addFileRecordToDB(const RString& filename, int32 dirid);   
    Path workdir_;
    RString lasterr_;
    Path dbfile_;
    DB* db_ = nullptr;
    Path pkgfile_;    
    DirTree* dirtree_ = nullptr;
    Table dirdbtable_;
    Table filedbtable_;
    Path tmpdatafile_;
    std::ofstream* tmpdatafilewriter_ = nullptr;
};

RATEL_NAMESPACE_END

#endif
