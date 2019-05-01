/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: serializer.h 
CreateTime: 2019-4-18 20:58
=========================================================================*/
#ifndef __serializer_h__
#define __serializer_h__

#include "smetacls.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SERIALIZATION_API SObject
{
public:
	virtual SMetaCls* getMetaCls()const{ return nullptr; }
	void serialize(Archive& ar);
	bool isSerializable()const;
	bool isKindOf(const SMetaCls* metacls)const;
	SObject();
	virtual ~SObject();
};

// const char* clsname = nullptr;
// int32_t objsize = 0;
// SMetaCls* basecls = nullptr;
// int32_t schema = -1;
// SObject* (*createfunc)() = nullptr;
// SObject* (*getbasefunc)() = nullptr;

RATEL_NAMESPACE_END

#define GET_METACLS(TheCls) (&TheCls::Cls##TheCls)

#define DECLARE_SERIALIZER(BaseCls, MyCls) \
	public:\
	static RATEL::SMetaCls Cls##MyCls;\
	RATEL::SMetaCls* getMetaCls()const;

#define IMPL_SERIALIZER(BaseCls, MyCls, Schema) \
	static const char* stClsName = #MyCls; \
	RATEL::SMetaCls MyCls::Cls##MyCls = { stClsName, sizeof(MyCls), GET_METACLS(BaseCls), Schema, nullptr, nullptr}; \
	RATEL::SMetaCls* getMetaCls()const{ return Cls##MyCls; }

#endif
