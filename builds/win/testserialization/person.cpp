/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testserialization 
Module: person.cpp 
CreateTime: 2019-6-7 21:32
=========================================================================*/
#include "person.h"
#include <iostream>
#include "archive.h"
USING_RATEL
using namespace std;

IMPL_SERIAL(Person, SObject, 1)

Person::Person()
{}

Person::Person(const RString& nameval)
	:name(nameval)
{}

Person::~Person()
{}

void Person::print()
{
	cout << "Name:" << name.cstr() << endl;
	cout << "Age:" << age << endl;
	cout << "Gender:" << gender << endl;
}

void Person::serialize(Archive& ar)
{
	if(ar.isStored()){
		uint32_t namelen = name.size();
		ar << namelen << name << age << gender;
	}else{
		uint32_t namelen = 0;
		ar >> namelen;
		name.resize(namelen);
		ar >> name >> age >> gender;
	}
}

