/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: smetacls.cpp 
CreateTime: 2019-4-25 22:01
=========================================================================*/
#include "smetacls.h"
#include "serializationlogger.h"

RATEL_NAMESPACE_BEGIN

SMetaCls* SMetaCls::stFirstCls = nullptr;

SObject* SMetaCls::createObject()
{
	if(fncreateobj == nullptr){
		slog_err(serializationlogger) << "trying create object failed, perhaps its' class declaration not label with 'DECL_DYNCREATE'!" << endl;
		return nullptr;
	}
	return (*fncreateobj)();
}

SMetaCls* SMetaCls::Load()
{
	return nullptr;
}

RATEL_NAMESPACE_END


