/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: fsyslogger.h 
CreateTime: 2018-12-23 21:00
=========================================================================*/
#ifndef __fsyslogger_h__
#define __fsyslogger_h__

#include "filesyspublic.h"
#include "liblogger.h"

DECL_LOGGER(filesys, RATEL_FILESYS_API)

#define fsyslogger liblogger(filesys)

#endif
