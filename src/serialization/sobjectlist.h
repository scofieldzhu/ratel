/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: sobjectlist.h 
CreateTime: 2019-6-1 19:46
=========================================================================*/
#ifndef __sobjectlist_h__
#define __sobjectlist_h__

#include "sobject.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SERIALIZATION_API SObjectList : public SObject
{
public:
	bool empty()const { return count() == 0; }
	uint32_t count()const { return (uint32_t)objects_.size(); }
	SObjectSPtr object(uint32_t index);
	void append(SObjectSPtr obj);
	void remove(SObjectSPtr obj);
	void serialize(Archive& ar);
	SObjectList();
	~SObjectList();

private:
	std::vector<SObjectSPtr> objects_;
};

RATEL_NAMESPACE_END

#endif