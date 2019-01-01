/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: basetypes.h 
=========================================================================*/
#ifndef __basetypes_h__
#define __basetypes_h__

#include <cstdint> // c++11
#include "ratelnsp.h"

RATEL_NAMESPACE_BEGIN

using int8 = ::int8_t;
using uint8 = ::uint8_t;
using int16 = ::int16_t;
using uint16 = ::uint16_t;
using int32 = ::int32_t;
using uint32 = ::uint32_t;
using int64 = ::int64_t;
using uint64 = ::uint64_t;

using intl8 = ::int_least8_t;
using uintl8 = ::uint_least8_t;
using intl16 = ::int_least16_t;
using uintl16 = ::uint_least16_t;
using intl32 = ::int_least32_t;
using uintl32 = ::uint_least32_t;
using intl64 = ::int_least64_t;
using uintl64 = ::uint_least64_t;

RATEL_NAMESPACE_END

#endif
