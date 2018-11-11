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
#include "liblogger.h"

DECL_LOGGER(package, RATEL_PACKAGE_API)

#define pkglogger liblogger(package)

#endif
