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

using ::int8_t;
using ::uint8_t;
using ::int16_t;
using ::uint16_t;
using ::int32_t;
using ::uint32_t;
using ::int64_t;
using ::uint64_t;

using Int8 = int8_t;
using uInt8 = uint8_t;
using Int16 = int16_t;
using uInt16 = uint16_t;
using Int32 = int32_t;
using uInt32 = uint32_t;
using Int64 = int64_t;
using uInt64 = uint64_t;

using ::int_least8_t;
using ::uint_least8_t;
using ::int_least16_t;
using ::uint_least16_t;
using ::int_least32_t;
using ::uint_least32_t;
using ::int_least64_t;
using ::uint_least64_t;

using Intl8 = int_least8_t;
using uIntl8 = uint_least8_t;
using Intl16 = int_least16_t;
using uIntl16 = uint_least16_t;
using Intl32 = int_least32_t;
using uIntl32 = uint_least32_t;
using Intl64 = int_least64_t;
using uIntl64 = uint_least64_t;

RATEL_NAMESPACE_END
#endif
