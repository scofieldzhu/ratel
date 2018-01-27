/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: kernelpublic.h 
=========================================================================*/
#ifndef __kernelpublic_h__
#define __kernelpublic_h__

#ifdef KERNEL_EXPORTS
    #define RATEL_KERNEL_API __declspec(dllexport)
#else
    #define RATEL_KERNEL_API __declspec(dllimport)
#endif

#include "basicpublic.h"

#endif
