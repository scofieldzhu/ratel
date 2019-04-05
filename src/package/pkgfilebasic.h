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

/*
pkg file structure as following:
----------Start Of File----------------
--------Type Code----------------------
--------DB File Name-------------------
--------DB File Data Size--------------
--------DB File Data-------------------
--------Dir Data Block File UID(1)-----
--------Dir Data Block File Size(1)----
--------Dir Data Block File Content(1)-
--------Dir Data Block File UID(2)-----
--------Dir Data Block File Size(2)----
--------Dir Data Block File Content(2)-
-
-
-
--------Dir Data Block File UID(n)-----
--------Dir Data Block File Size(n)----
--------Dir Data Block File Content(n)-
----------End Of File------------------
*/

RATEL_NAMESPACE_END

#endif
