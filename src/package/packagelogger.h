/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: packagelogger.h 
=========================================================================*/
#ifndef __packagelogger_h__
#define __packagelogger_h__

#include "packagepublic.h"
#include "utilmacro.h"
#include "ratelnsp.h"

RATEL_NAMESPACE_BEGIN
namespace package {
    SFLOGGER::Logger* LocalLogger();
    RATEL_PACKAGE_API void SetLocalLogger(SFLOGGER::Logger& l);
}
#define packagelogger RATEL::package::LocalLogger()
RATEL_NAMESPACE_END
#endif
