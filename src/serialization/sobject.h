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

#include "sclsmeta.h"
#include "clsmetaautoconf.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SERIALIZATION_API SObject
{
public:
	static SClsMeta st_SObject_Meta;
	virtual SClsMeta& getClsMeta()const{ return SObject::st_SObject_Meta; }
	virtual void serialize(Archive& ar);
	bool isSerializable()const;
	bool isKindOf(const SObject&);
	SObject();
	virtual ~SObject();
};

RATEL_NAMESPACE_END

#define GET_CLS_META(TheCls) (TheCls::st_##TheCls##_Meta)
#define GET_CLS_META_PTR(TheCls) std::addressof(GET_CLS_META(TheCls)) 

#define __DECL_NEW_METACLS(MyCls) \
	public:\
	static RATEL::SClsMeta st_##MyCls##_Meta;\
	RATEL::SClsMeta& getClsMeta()const; \
	static MyCls* SafeCast(RATEL::SObject*);

#define __IMPL_NEW_METACLS(MyCls, BaseCls, Schema, fnNew) \
	static const char* stClsName = #MyCls; \
	RATEL::SClsMeta MyCls::st_##MyCls##_Meta = {stClsName, sizeof(MyCls), Schema, fnNew, GET_CLS_META_PTR(BaseCls), nullptr}; \
	static RATEL::ClsMetaAutoConf __autoconf(GET_CLS_META_PTR(MyCls)); \
	RATEL::SClsMeta& MyCls::getClsMeta()const{ return GET_CLS_META(MyCls); } \
	MyCls* MyCls::SafeCast(RATEL::SObject* obj){ \
		if(obj){ \
			if(GET_CLS_META(MyCls).isBase(obj->getClsMeta())) \
				return static_cast<MyCls*>(obj); \
		} \
		return nullptr; \
	}

#define DECL_DYNAMIC(MyCls) __DECL_NEW_METACLS(MyCls)

#define IMPL_DYNAMIC(MyCls, BaseCls) __IMPL_NEW_METACLS(MyCls, BaseCls, 0xFFFF, nullptr)	

#define DECL_DYNCREATE(MyCls) \
	DECL_DYNAMIC(MyCls) \
	static RATEL::SObject* CreateObject();

#define IMPL_DYNCREATE(MyCls, BaseCls) \
	RATEL::SObject* MyCls::CreateObject() { return new MyCls(); } \
	__IMPL_NEW_METACLS(MyCls, BaseCls, 0xFFFF, MyCls::CreateObject) 

#define DECL_SERIAL(MyCls) \
	DECL_DYNCREATE(MyCls) \
	friend RATEL::Archive& operator>>(RATEL::Archive& ar, MyCls*& myobj);

#define IMPL_SERIAL(MyCls, BaseCls, Schema) \
	RATEL::SObject* MyCls::CreateObject() { return new MyCls(); } \
	__IMPL_NEW_METACLS(MyCls, BaseCls, Schema, MyCls::CreateObject) \
	RATEL::Archive& operator>>(RATEL::Archive& ar, MyCls*& myobj){ \
		myobj = (MyCls*)ar.readObject(GET_CLS_META(MyCls)); \
		return ar; \
	}
	
#endif
