/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: basic_export.h 
Create time: 2023/11/30 09:28:45
=========================================================================*/
#ifndef __basic_export_h__
#define __basic_export_h__

#include "ratel/basic/cross_platform.h"
#include "ratel/basic/ratel_nsp.h"

#ifdef PLATFORM_WIN
    #ifdef BASIC_EXPORTS
        #define RATEL_BASIC_API __declspec(dllexport)
    #else
        #define RATEL_BASIC_API __declspec(dllimport)
    #endif
#endif

#endif
