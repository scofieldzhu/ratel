/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: databaselogger.h 
CreateTime: 2019-7-30 21:14
=======================================================================*/
#ifndef __databaselogger_h__
#define __databaselogger_h__

#include "databasepublic.h"
#include "liblogger.h"

DECL_LOGGER(database, RATEL_DATABASE_API)

#define databaselogger liblogger(database)

#endif