/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: kernellogger.h 
CreateTime: 2019-1-6 21:30
=========================================================================*/
#ifndef __kernellogger_h__
#define __kernellogger_h__

#include "kernelpublic.h"
#include "liblogger.h"

DECL_LOGGER(kernel, RATEL_KERNEL_API)

#define kernellogger liblogger(kernel)

#endif
