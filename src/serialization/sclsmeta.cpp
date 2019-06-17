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

SObjectSPtr SClsMeta::createObject()const
{
	if(fncreateobj == nullptr){
		slog_err(serializationlogger) << "trying create object failed, perhaps its' class declaration not label with 'DECL_DYNCREATE'!" << endl;
		return nullptr;
	}
	return (*fncreateobj)();
}

bool SClsMeta::isBase(const SClsMeta& rhs) const
{
	const SClsMeta* curmeta = this;
	while(curmeta != nullptr){
		if(curmeta == &rhs)
			return true;
		curmeta = curmeta->basecls;
	}
	return false;
}

void SClsMeta::store(Archive& ar)const
{
	uint32_t len = (uint32_t)strlen(clsname);
	ar << schemano << len;
	ar.writeData(clsname, len);
}

SClsMeta* SClsMeta::LoadMeta(Archive& ar)
{
	uint32_t namelen = 0;
	uint32_t oldschemano = 0;
	ar >> oldschemano >> namelen;
	char* clsname = new char[namelen + 1];
	SClsMeta* resultmeta = nullptr;
	if(!ar.readData(clsname, namelen, nullptr))
		slog_err(serializationlogger) << "invalid file format!" << endl;
	else{
		clsname[namelen] = '\0';
		resultmeta = SClsMeta::FindMeta(clsname);
		if(resultmeta == nullptr)
			slog_err(serializationlogger) << "unknown class(" << (const char*)clsname << ")!" << endl;
	}			
	rtarrydelete(clsname);
	return resultmeta;
}

SObjectSPtr SClsMeta::CreateObject(const char* clsname)
{
	SClsMeta* thiscls = FindMeta(clsname);
	return thiscls ? thiscls->createObject() : nullptr;	
}

SClsMeta* SClsMeta::FindMeta(const char* clsname)
{
	SClsMeta* curcls = stFirstCls;
	while(curcls != nullptr){
		if(strcmp(curcls->clsname, clsname) == 0)
			return curcls;
		curcls = curcls->nextcls;
	}
	return nullptr;
}

SClsMetaInitializer::SClsMetaInitializer(SClsMeta* newcls)
{
	newcls->nextcls = SClsMeta::stFirstCls; 
	SClsMeta::stFirstCls = newcls; 
}

SClsMetaInitializer::~SClsMetaInitializer()
{}

RATEL_NAMESPACE_END