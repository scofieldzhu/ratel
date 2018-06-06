/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: basiclogger.h 
=========================================================================*/
#ifndef __basiclogger_h__
#define __basiclogger_h__

#include "liblogger.h"
#include "basicpublic.h"

DECL_LOGGER(basic, RATEL_BASIC_API)

#define basiclogger liblogger(basic)

#endif
