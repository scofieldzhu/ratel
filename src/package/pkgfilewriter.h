/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: pkgfilewriter.h 
CreateTime: 2018-9-9 17:57
=========================================================================*/
#ifndef __pkgfilewriter_h__
#define __pkgfilewriter_h__

#include <fstream>
#include "packagepublic.h"
#include "path.h"

RATEL_NAMESPACE_BEGIN

class RATEL_PACKAGE_API PkgFileWriter
{
public:
    bool beginWrite();
    bool writeFileData(const Path& file);
    void end();
    bool inWritting()const;
    PkgFileWriter(const Path& filepath);
    ~PkgFileWriter();

private:
    std::ofstream ofs_;
    const Path& filepath_;
};

RATEL_NAMESPACE_END
#endif
