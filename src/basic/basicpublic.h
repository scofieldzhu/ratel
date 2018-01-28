/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: basicpublic.h 
=========================================================================*/
#ifndef __basicpublic_h__
#define __basicpublic_h__

#ifdef BASIC_EXPORTS
    #define RATEL_BASIC_API __declspec(dllexport)
#else
    #define RATEL_BASIC_API __declspec(dllimport)
#endif

#include "basetypes.h"

#endif
