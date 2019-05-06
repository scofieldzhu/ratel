/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: metaclsautoconf.h 
CreateTime: 2019-5-5 22:22
=========================================================================*/
#ifndef __metaclsautoconf_h__
#define __metaclsautoconf_h__

#include "serializationpublic.h"

RATEL_NAMESPACE_BEGIN

struct RATEL_SERIALIZATION_API MetaClsAutoConf
{
	MetaClsAutoConf(SMetaCls* newcls);
	~MetaClsAutoConf();
};
RATEL_NAMESPACE_END

#endif
