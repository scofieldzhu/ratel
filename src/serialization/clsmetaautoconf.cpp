/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: clsmetaautoconf.cpp 
CreateTime: 2019-5-5 22:25
=========================================================================*/
#include "clsmetaautoconf.h"
#include "sclsmeta.h"

RATEL_NAMESPACE_BEGIN

ClsMetaAutoConf::ClsMetaAutoConf(SClsMeta* newcls)
{
	newcls->nextcls = SClsMeta::stFirstCls;
	SClsMeta::stFirstCls = newcls;
}

ClsMetaAutoConf::~ClsMetaAutoConf()
{}

RATEL_NAMESPACE_END