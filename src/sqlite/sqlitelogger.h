/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqlitelogger.h 
CreateTime: 2018-7-14 10:36
=========================================================================*/
#ifndef __sqlitelogger_h__
#define __sqlitelogger_h__

#include "sqlitepublic.h"
#include "liblogger.h"

DECL_LOGGER(sqlite, RATEL_SQLITE_API)

#define sqlitelogger liblogger(sqlite)

#endif
