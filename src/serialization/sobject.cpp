/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: sobject.cpp 
CreateTime: 2019-5-5 21:39
=========================================================================*/
#include "sobject.h"

RATEL_NAMESPACE_BEGIN

static const char* stSObject = "SObject";
SMetaCls SObject::ClsSObject = {stSObject, sizeof(stSObject), -1, nullptr, nullptr, nullptr};

SObject::SObject(){}

SObject::~SObject(){}

void SObject::serialize(Archive& ar)
{

}

bool SObject::isSerializable() const
{
	return false;
}

bool SObject::isKindOf(const SMetaCls* targetmetacls) const
{
	SMetaCls* curmetacls = getMetaCls();
	while(curmetacls != nullptr){
		if(curmetacls == targetmetacls)
			return true;
		curmetacls = curmetacls->basecls;
	}
	return false;
}

RATEL_NAMESPACE_END
