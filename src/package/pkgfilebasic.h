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

const uint32 kPkgTypeCharNum = 6;

struct PkgInfo
{
    char pkgtype[kPkgTypeCharNum]; //rtlpkg
    uint32 dbdatasize;
    char dbdata[1]; //datasize
};

RATEL_NAMESPACE_END

#endif
