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

struct RATEL_SERIALIZATION_API SClsMeta
{
	const char* clsname = nullptr;
	uint32_t objsize = 0;
	uint32_t schemano = 0xFFFFF;
	SObjectSPtr (*fncreateobj)() = nullptr;
	SClsMeta* basecls = nullptr;
	
	SObjectSPtr createObject()const;

	bool isBase(const SClsMeta&)const;

	void store(Archive& ar)const;	

	static SObjectSPtr CreateObject(const char* clsname);
	static SClsMeta* FindMeta(const char* clsname);
	static SClsMeta* LoadMeta(Archive& ar);

	static SClsMeta* stFirstCls;	
	SClsMeta* nextcls = nullptr;		
};

struct RATEL_SERIALIZATION_API SClsMetaInitializer
{
	SClsMetaInitializer(SClsMeta* newcls);
	~SClsMetaInitializer();
};

RATEL_NAMESPACE_END

#endif