/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: filesyspublic.h 
=========================================================================*/
#ifndef __filesyspublic_h__
#define __filesyspublic_h__

#include "basic.h"

#ifdef FILESYS_EXPORTS
    #define RATEL_FILESYS_API __declspec(dllexport)
#else
    #define RATEL_FILESYS_API __declspec(dllimport)
#endif

RATEL_NAMESPACE_BEGIN
class Path;
class DirWalker;
RATEL_NAMESPACE_END

#endif
