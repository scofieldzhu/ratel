/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: databasepublic.h 
CreateTime: 2019-7-30 21:15
=======================================================================*/
#ifndef __databasepublic_h__
#define __databasepublic_h__

#ifdef DATABASE_EXPORTS
    #define RATEL_DATABASE_API __declspec(dllexport)
#else
    #define RATEL_DATABASE_API __declspec(dllimport)
#endif

#include "basic.h"

#endif
