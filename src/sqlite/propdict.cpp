/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: propdict.cpp 
CreateTime: 2019-1-26 19:23
=========================================================================*/
#include "propdict.h"
#include "sqlitelogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

PropDict::PropDict()
{}

PropDict::PropDict(const vector<PropKey>& keys)
{
	for(const PropKey& k : keys)
		propstatusdict_.insert({k, false});
}

PropDict::PropDict(std::initializer_list<PropKey> llkeys)
{
	for(auto it = llkeys.begin(); it != llkeys.end(); ++it)
		propstatusdict_.insert({*it, false});
}

PropDict::~PropDict()
{}

bool PropDict::existProp(const PropKey& key) const
{
	return propstatusdict_.find(key) != propstatusdict_.end();
}

PropDict& PropDict::setPropStatusOn(const PropKey& key)
{
	setPropStatus(key, true); 
	return *this;
}

PropDict& PropDict::setPropStatusOff(const PropKey& key)
{
	setPropStatus(key, false); 
	return *this;
}

void PropDict::setPropStatus(const PropKey& key, bool f)
{
	if(existProp(key))
		propstatusdict_[key] = f;
}

bool PropDict::getPropStatus(const PropKey& key) const
{
	if(existProp(key))
		return propstatusdict_.at(key);
	slog_warn(sqlitelogger) << "Key[" << key.cstr() << "] not exists!" << endl;
	return false;	
}

RATEL_NAMESPACE_END