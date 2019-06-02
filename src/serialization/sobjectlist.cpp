/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: sobjectlist.cpp 
CreateTime: 2019-6-1 19:46
=========================================================================*/
#include "sobjectlist.h"
#include "archive.h"
#include "serializationlogger.h"

RATEL_NAMESPACE_BEGIN

SObjectList::SObjectList()
{}

SObjectList::~SObjectList()
{}

SObjectSPtr SObjectList::object(uint32_t index)
{
	return index < count() ? objects_[index] : nullptr; 
}

void SObjectList::append(SObjectSPtr obj)
{
	if(std::find(objects_.begin(), objects_.end(), obj) == objects_.end())
		objects_.push_back(obj);
}

void SObjectList::remove(SObjectSPtr obj)
{
	auto it = std::find(objects_.begin(), objects_.end(), obj);
	if(it != objects_.end())
		objects_.erase(it);
}

void SObjectList::serialize(Archive& ar)
{
	if(ar.isStored()){
		uint32_t count = objects_.size();
		ar << count;
		for(auto it : objects_)
			ar << *it;
	}else{
		uint32_t count = 0;
		ar >> count;
		if(count){
			objects_.clear();
			while(count--){
				SObjectSPtr newobj = ar.readNextObject();
				if(newobj == nullptr){
					slog_fatal(serializationlogger) << "Read SObject failed for wrong format!" << endl;
					return;
				}
				objects_.push_back(newobj);
			}
		}
	}
}

RATEL_NAMESPACE_END