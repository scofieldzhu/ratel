/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: rstring.h 
=========================================================================*/
#ifndef __rstring_h__
#define __rstring_h__

#include "basicpublic.h"
#include <string>

RATEL_NAMESPACE_BEGIN

class RATEL_BASIC_API RString : public std::basic_string<char>
{
public:
    RString();
};

RATEL_NAMESPACE_END

#endif
