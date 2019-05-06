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
#include "metaclsautoconf.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SERIALIZATION_API SObject
{
public:
	static SMetaCls ClsSObject;
	virtual SMetaCls* getMetaCls()const{ return &SObject::ClsSObject; }
	void serialize(Archive& ar);
	bool isSerializable()const;
	bool isKindOf(const SMetaCls* metacls)const;
	SObject();
	virtual ~SObject();
};

RATEL_NAMESPACE_END

#define GET_METACLS(TheCls) (&TheCls::Cls##TheCls)

#define __DECL_NEW_METACLS(MyCls) \
	public:\
	static RATEL::SMetaCls Cls##MyCls;\
	RATEL::SMetaCls* getMetaCls()const;

#define __IMPL_NEW_METACLS(MyCls, BaseCls, Schema, fnNew) \
	static const char* stClsName = #MyCls; \
	RATEL::SMetaCls MyCls::Cls##MyCls = { stClsName, sizeof(MyCls), Schema, fnNew, GET_METACLS(BaseCls), nullptr}; \
	static RATEL::MetaClsAutoConf __autoconf(&MyCls::Cls##MyCls); \
	RATEL::SMetaCls* MyCls::getMetaCls()const{ return &MyCls::Cls##MyCls; }

#define DECL_DYNAMIC(MyCls) __DECL_NEW_METACLS(MyCls)

#define IMPL_DYNAMIC(MyCls, BaseCls) __IMPL_NEW_METACLS(MyCls, BaseCls, -1, nullptr)	

#define DECL_DYNCREATE(MyCls) \
	DECL_DYNAMIC(MyCls) \
	static RATEL::SObject* CreateObject();

#define IMPL_DYNCREATE(MyCls, BaseCls) \
	RATEL::SObject* MyCls::CreateObject() { return new MyCls(); } \
	__IMPL_NEW_METACLS(MyCls, BaseCls, -1, MyCls::CreateObject) 

#define DECL_SERIAL(MyCls) \
	DECL_DYNCREATE(MyCls) \
	friend RATEL::Archive& operator>>(RATEL::Archive& ar, MyCls*& myobj);

#define IMPL_SERIAL(MyCls, BaseCls, Schema) \
	RATEL::SObject* MyCls::CreateObject() { return new MyCls(); } \
	__IMPL_NEW_METACLS(MyCls, BaseCls, Schema, MyCls::CreateObject) \
	RATEL::Archive& operator>>(RATEL::Archive& ar, MyCls*& myobj){ \
		myobj = (MyCls*)ar.readObject(GET_METACLS(MyCls)); \
		return ar; \
	}
	
#endif
