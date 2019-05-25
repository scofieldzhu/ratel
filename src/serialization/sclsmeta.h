/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: sclsmeta.h 
CreateTime: 2019-4-18 21:15
=========================================================================*/
#ifndef __sclsmeta_h__
#define __sclsmeta_h__

#include "serializationpublic.h"

RATEL_NAMESPACE_BEGIN
                                         
struct SClsMeta
{
	const char* clsname = nullptr;
	uint32_t objsize = 0;
	uint32_t schemano = -1;
	SObject* (*fncreateobj)() = nullptr;
	SClsMeta* basecls = nullptr;
	
	SObject* createObject();

	void store(Archive& ar)const;

	static SObject* CreateObject(const char* clsname);
	static SClsMeta* Find(const char* clsname);

	static SClsMeta* stFirstCls;	
	SClsMeta* nextcls = nullptr;		
};

RATEL_NAMESPACE_END

#endif