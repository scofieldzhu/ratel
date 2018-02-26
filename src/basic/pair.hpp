/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: pair.hpp 
=========================================================================*/
#ifndef __pair_hpp__
#define __pair_hpp__

#include "basetypes.h"

RATEL_NAMESPACE_BEGIN

template <class K, class V>
struct Pair
{
    K key;
    V value;
};


RATEL_NAMESPACE_END

#endif
