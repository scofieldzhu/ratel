/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testserialization 
Module: person.h 
CreateTime: 2019-5-26 21:32
=========================================================================*/
#ifndef __person_h__
#define __person_h__

#include "sobject.h"
#include "rstring.h"

class Person : public RATEL::SObject
{
	DECL_SERIAL(Person)
public:
	void print();
	void serialize(RATEL::Archive& ar);
	Person();
	Person(const RATEL::RString&);
	~Person();
	RATEL::RString name = "unnamed";
	uint32_t age = 0;
	double salary = 18900.0;
};

#endif
