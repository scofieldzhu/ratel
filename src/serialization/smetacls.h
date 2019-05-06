/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: smetacls.h 
CreateTime: 2019-4-18 21:15
=========================================================================*/
#ifndef __smetacls_h__
#define __smetacls_h__

#include "serializationpublic.h"

RATEL_NAMESPACE_BEGIN
                                         
struct SMetaCls
{
	const char* clsname = nullptr;
	int32_t objsize = 0;
	int32_t schema = -1;
	SObject* (*fncreateobj)() = nullptr;
	SMetaCls* basecls = nullptr;
	
	SObject* createObject();
	static SMetaCls* Load();	

	static SMetaCls* stFirstCls;	
	SMetaCls* nextcls = nullptr;		
};

RATEL_NAMESPACE_END

#endif