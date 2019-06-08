/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testserialization 
Module: teacher.cpp 
CreateTime: 2019-6-7 21:51
=========================================================================*/
#include "teacher.h"
#include "archive.h"
#include <iostream>
using namespace std;

IMPL_SERIAL(Teacher, Person, 1)

Teacher::Teacher()
{}

Teacher::Teacher(const RATEL::RString& str)
	:Person(str)
{}

void Teacher::print()
{
	cout <<"---------teacher info------------" << endl;
	Person::print();
	cout << "tid:" << tid.cstr() << endl;
	cout << "salary:" << salary << endl;
	cout <<"---------------------------------" << endl;
}

void Teacher::serialize(RATEL::Archive& ar)
{
	Person::serialize(ar);
	if(ar.isStored()){
		uint16_t len = tid.size();
		ar << len << tid << salary;
	}else{
		uint16_t len = 0;
		ar >> len;
		tid.resize(len);
		ar >> tid >> salary; 
	}
}

