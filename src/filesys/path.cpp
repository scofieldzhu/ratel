/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: path.cpp 
CreateTime: 2018-6-9 20:24
=========================================================================*/
#include "path.h"

RATEL_NAMESPACE_BEGIN
const char Path::Separator = '/';

Path::Path(const RString& str)
    :pathstr_(str)
{}

Path::~Path()
{}

Path Path::join(const RString& str) const
{
    return Path("");

}

Path Path::parentDir() const
{
    return Path("");
}
RATEL_NAMESPACE_END
