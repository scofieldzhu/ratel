/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: basicpublic.h 
=========================================================================*/
#ifndef __basicpublic_h__
#define __basicpublic_h__

#include "crossplatform.h"

#ifdef PLATFORM_WIN
    #ifdef BASIC_EXPORTS
        #define RATEL_BASIC_API __declspec(dllexport)
    #else
        #define RATEL_BASIC_API __declspec(dllimport)
    #endif
#endif

#include <cstdint> // c++11 ffg
#include <vector>
#include <map>
#include <list>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <sstream>
#include <fstream>
#include "langext.h"

#endif
