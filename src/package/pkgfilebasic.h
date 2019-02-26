/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: pkgfilebasic.h 
CreateTime: 2018-9-2 21:01
=========================================================================*/
#ifndef __pkgfilebasic_h__
#define __pkgfilebasic_h__

#include "packagepublic.h"

RATEL_NAMESPACE_BEGIN

const uint32_t kPkgTypeCharNum = 6;

struct PkgInfo
{
    char pkgtypecode[kPkgTypeCharNum]; //rtlpkg
    int32_t compressor; //
    int32_t compressrate;
    uint32_t dbdatasize;
    char dbdata[1]; //datasize
};

#define PKGTYPE_CODE "rtlpkg"

RATEL_NAMESPACE_END

#endif
