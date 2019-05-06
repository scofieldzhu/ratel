/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: metaclsautoconf.cpp 
CreateTime: 2019-5-5 22:25
=========================================================================*/
#include "metaclsautoconf.h"
#include "smetacls.h"

RATEL_NAMESPACE_BEGIN

MetaClsAutoConf::MetaClsAutoConf(SMetaCls* newcls)
{
	newcls->nextcls = SMetaCls::stFirstCls;
	SMetaCls::stFirstCls = newcls;
}

MetaClsAutoConf::~MetaClsAutoConf()
{}

RATEL_NAMESPACE_END