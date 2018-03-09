/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: packagelogger.cpp 
=========================================================================*/
#include "packagelogger.h"



RATEL_NAMESPACE_BEGIN
USING_SFLOGGER;

Logger* st_LocalLogger = nullptr;

namespace package {

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