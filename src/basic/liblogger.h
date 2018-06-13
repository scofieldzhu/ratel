/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: liblogger.h 
=========================================================================*/
#ifndef __liblogger_h__
#define __liblogger_h__

#include "utilmacro.h"
#include "ratelnsp.h"

RATEL_NAMESPACE_BEGIN

#define DECL_LOGGER(lib, exportmacro)\
    namespace RATEL{ \
        namespace lib{\
                exportmacro SFLOGGER::Logger* LibLogger(); \
                exportmacro void SetLogger(SFLOGGER::Logger* l); \
        }\
    }

#define IMPL_LOGGER(lib) \
    namespace RATEL{ \
        namespace lib{\
            SFLOGGER::Logger* g_ThisLogger = nullptr;\
            SFLOGGER::Logger* LibLogger(){ return g_ThisLogger; } \
            void SetLogger(SFLOGGER::Logger* l){ g_ThisLogger = l; } \
        }\
    }

#define liblogger(lib) RATEL::lib::LibLogger()

using std::endl;

RATEL_NAMESPACE_END
#endif
