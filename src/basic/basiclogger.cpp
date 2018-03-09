/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: basiclogger.cpp 
=========================================================================*/
#include "basiclogger.h"

RATEL_NAMESPACE_BEGIN

USING_SFLOGGER;

Logger* st_LocalLogger = nullptr;

namespace basic{
    
    Logger* LocalLogger()
    {
        return st_LocalLogger;
    }

    void SetLocalLogger(Logger& l)
    {
        st_LocalLogger = &l;
    }
}

RATEL_NAMESPACE_END
