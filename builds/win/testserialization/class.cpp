/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testserialization 
Module: class.cpp 
CreateTime: 2019-6-8 15:17
=========================================================================*/
#include "class.h"
#include <iostream>
#include "student.h"
#include "teacher.h"
#include "archive.h"
USING_RATEL
using namespace std;

IMPL_SERIAL(Class, SObject, 1)

Class::Class()
{}

Class::Class(const RATEL::RString& nval)
	:name(nval)
{}

Class::~Class()
{}

void Class::print()
{
	cout << "============class statistics====================" << endl;
	cout << "Name:" <<name.cstr() << endl;
	cout << ">>>>>>>>>>>>>>Teachers:" << endl;
	for(int32_t i = 0; i < teachers.count(); ++i){
		Person* p = Person::SafeCast(teachers.object(i).get());
		p->print();
	}
	cout << ">>>>>>>>>>>>>>Students:" << endl;
	for(int32_t i = 0; i < students.count(); ++i){
		Person* p = Person::SafeCast(students.object(i).get());
		p->print();
	}
	cout << "==============================================" << endl;
}

void Class::serialize(RATEL::Archive& ar)
{
	if(ar.isStored()){
		uint16_t len = name.size();
		ar << len << name;
		teachers.serialize(ar);
		students.serialize(ar);
	}else{
		uint16_t len = 0;
		ar >> len;
		name.resize(len);
		ar >> name;
		teachers.serialize(ar);
		students.serialize(ar);
	}
}