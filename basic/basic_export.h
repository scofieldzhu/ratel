/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: basic_public.h 
=========================================================================*/
#ifndef __basic_public_h__
#define __basic_public_h__

#include "crossplatform.h"
#include "ratel_nsp.h"

#ifdef PLATFORM_WIN
    #ifdef BASIC_EXPORTS
        #define RATEL_BASIC_API __declspec(dllexport)
    #else
        #define RATEL_BASIC_API __declspec(dllimport)
    #endif
#endif

// #include <cstdint> // c++11 
// #include <vector>
// #include <map>
// #include <list>
// #include <string>
// #include <algorithm>
// #include <memory>
// #include <functional>

#endif
