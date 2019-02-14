/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: propdict.h 
CreateTime: 2019-1-26 19:15
=========================================================================*/
#ifndef __propdict_h__
#define __propdict_h__

#include "kernelpublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_KERNEL_API PropDict
{
public:
	typedef RString PropKey;
	bool existProp(const PropKey& key)const;
	PropDict& setPropStatusOn(const PropKey& key);
	PropDict& setPropStatusOff(const PropKey& key);
	void setPropStatus(const PropKey& key, bool f);
	bool getPropStatus(const PropKey& key)const;
	PropDict();
	PropDict(const std::vector<PropKey>& keys);
	PropDict(std::initializer_list<PropKey> llkeys);
	~PropDict();

private:
	std::map<PropKey, bool> propstatusdict_;
};

RATEL_NAMESPACE_END

#endif
