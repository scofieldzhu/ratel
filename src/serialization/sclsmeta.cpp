/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: smetacls.cpp 
CreateTime: 2019-4-25 22:01
=========================================================================*/
#include "sclsmeta.h"
#include "serializationlogger.h"
#include "archive.h"

RATEL_NAMESPACE_BEGIN

SClsMeta* SClsMeta::stFirstCls = nullptr;

SObject* SClsMeta::createObject()const
{
	if(fncreateobj == nullptr){
		slog_err(serializationlogger) << "trying create object failed, perhaps its' class declaration not label with 'DECL_DYNCREATE'!" << endl;
		return nullptr;
	}
	return (*fncreateobj)();
}

void SClsMeta::store(Archive& ar)const
{
	uint32_t len = strlen(clsname);
	ar << schemano << len;
	ar.writeData(clsname, len);
}

void SClsMeta::load(Archive& ar) const
{
	
}

SObject* SClsMeta::CreateObject(const char* clsname)
{
	SClsMeta* thiscls = Find(clsname);
	return thiscls ? thiscls->createObject() : nullptr;	
}

SClsMeta* SClsMeta::Find(const char* clsname)
{
	SClsMeta* curcls = stFirstCls;
	while(curcls != nullptr){
		if(strcmp(curcls->clsname, clsname) == 0)
			return curcls;
		curcls = curcls->nextcls;
	}
	return nullptr;
}

RATEL_NAMESPACE_END


