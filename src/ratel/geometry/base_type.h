/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.geometry
Module: base_type.h
Create time: 2023/12/08 08:48:45
=========================================================================*/
#ifndef __base_type_h__
#define __base_type_h__

#include <vector>
#include "ratel/basic/ratel_nsp.h"

RATEL_NAMESPACE_BEGIN

using Byte = char;
using BytePtr = Byte*;
using ConsBytePtr = const Byte*;
using ByteVec = std::vector<Byte>;

RATEL_NAMESPACE_END

#endif // base_type_h
