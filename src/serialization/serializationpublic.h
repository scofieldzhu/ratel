/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: serializationpublic.h 
CreateTime: 2019-4-18 21:20
=========================================================================*/
#ifndef __serializationpublic_h__
#define __serializationpublic_h__

#include "basic.h"

#ifdef SERIALIZATION_EXPORT
	#define RATEL_SERIALIZATION_API __declspec(dllexport)
#else 
	#define RATEL_SERIALIZATION_API __declspec(dllimport)
#endif

RATEL_NAMESPACE_BEGIN

struct SClsMeta;
class SObject;
class Archive;

RATEL_NAMESPACE_END

#endif
