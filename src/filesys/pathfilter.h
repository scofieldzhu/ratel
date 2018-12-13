/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: pathfilter.h 
CreateTime: 2018-12-13 20:35
=========================================================================*/
#ifndef __pathfilter_h__
#define __pathfilter_h__

#include "filesyspublic.h"

RATEL_NAMESPACE_BEGIN

class PathFilter
{
public:
    virtual bool filter(const Path& path)const = 0;
    virtual ~PathFilter() = default;
};

RATEL_NAMESPACE_END

#endif
