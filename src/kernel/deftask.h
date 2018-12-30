/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: deftask.h 
=========================================================================*/
#ifndef __deftask_h__
#define __deftask_h__

#include "kernelpublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_KERNEL_API Task 
{
public:
    virtual int32 execute() = 0;
    virtual ~Task() {};
};
RATEL_NAMESPACE_END

#endif
