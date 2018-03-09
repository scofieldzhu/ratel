/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: basiclogger.h 
=========================================================================*/
#ifndef __basiclogger_h__
#define __basiclogger_h__

#include "utilmacro.h"
#include "ratelnsp.h"

RATEL_NAMESPACE_BEGIN
namespace basic {
    SFLOGGER::Logger* LocalLogger();
    void SetLocalLogger(SFLOGGER::Logger& l);
}
#define basiclogger RATEL::basic::LocalLogger()
RATEL_NAMESPACE_END
#endif
