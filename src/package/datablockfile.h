/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.package
Module: datablockfile.h
CreateTime: 2018-12-30 20:09
=========================================================================*/
#ifndef __datablockfile_h__
#define __datablockfile_h__

#include "packagepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_PACKAGE_API DataBlockFile
{
public:
    void addDataBlock(uint32 blockid, );
    bool open();
    DataBlockFile(const char* file);
    DataBlockFile(const std::string file);
    ~DataBlockFile();
};

RATEL_NAMESPACE_END

#endif
