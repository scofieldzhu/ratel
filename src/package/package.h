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

#include "dirtable.h"
#include "filetable.h"
#include "path.h"

RATEL_NAMESPACE_BEGIN
class DB;
class DataBlockFile;
class RATEL_PACKAGE_API Package 
{
public:
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
    int32_t writeNewFileData(const Path& sourcefile);
    //int32_t addFileRecordToDB(const RString& filename, int32_t dirid);   
    Path generateDBFilePath()const;
    Path generateTmpDataFilePath()const;
    Path workdir_;
    Path dbfile_;
    DB* db_ = nullptr;
    Path pkgfile_;    
    DirTable dirtab_;
    FileTable filetab_;
    Path tmpdatafile_;
    std::ofstream* tmpdatafilewriter_ = nullptr;
	DataBlockFile* filedatastream_ = nullptr;
};

RATEL_NAMESPACE_END

#endif
