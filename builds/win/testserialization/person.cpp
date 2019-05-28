/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testserialization 
Module: person.cpp 
CreateTime: 2019-5-26 21:32
=========================================================================*/
#include "person.h"
#include "archive.h"
USING_RATEL

IMPL_SERIAL(Person, SObject, 1)

Person::Person()
{}

Person::Person(const RString& nameval)
	:name(nameval)
{}

Person::~Person()
{}

void Person::serialize(Archive& ar)
{
	if(ar.isStored()){
		uint32_t namelen = name.size();
		ar << namelen << name << age;
	}else{
		uint32_t namelen = 0;
		ar >> namelen >> name >>age;
	}
}

