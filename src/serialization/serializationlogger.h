/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: serializationlogger.h 
CreateTime: 2019-4-18 21:19
=========================================================================*/
#ifndef __serializationlogger_h__
#define __serializationlogger_h__

#include "serializationpublic.h"
#include "liblogger.h"

DECL_LOGGER(serialization, RATEL_SERIALIZATION_API)

#define serializationlogger liblogger(serialization)

#endif
