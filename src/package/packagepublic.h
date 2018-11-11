/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: packagepublic.h 
CreateTime:2018-6-6 19:51
=========================================================================*/
#ifndef __packagepublic_h__
#define __packagepublic_h__

#include "basic.h"
#include "filesyspublic.h"

#ifdef PACKAGE_EXPORTS
    #define RATEL_PACKAGE_API __declspec(dllexport)
#else
    #define RATEL_PACKAGE_API __declspec(dllimport)
#endif

RATEL_NAMESPACE_BEGIN
struct DirNode;
struct FileNode;
#define PATH_SEPARATOR '/'
#define ROOTDIR_NAME "*"
RATEL_NAMESPACE_END

#endif
