/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: packagepublic.h 
=========================================================================*/
#ifndef __packagepublic_h__
#define __packagepublic_h__

#ifdef PACKAGE_EXPORTS
    #define RATEL_PACKAGE_API __declspec(dllexport)
#else
    #define RATEL_PACKAGE_API __declspec(dllimport)

#endif
