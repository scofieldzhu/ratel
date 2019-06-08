/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testserialization 
Module: student.cpp 
CreateTime: 2019-6-7 19:22
=========================================================================*/
#include "student.h"
#include <iostream>
#include "archive.h"
using namespace std;

IMPL_SERIAL(Student, Person, 1)

Student::Student()
{}

Student::Student(const RATEL::RString& nstr)
	:Person(nstr)
{}

Student::~Student()
{}

void Student::print()
{
	cout <<"---------student info------------" << endl;
	Person::print(); 
	cout << "sid:" << sid.cstr() << endl;
	cout << "score:" << score << endl;
	cout <<"---------------------------------" << endl;
}

void Student::serialize(RATEL::Archive& ar)
{
	Person::serialize(ar);
	if(ar.isStored()){
		uint32_t idlen = sid.size();
		ar << idlen << sid << score;
	}else{
		uint32_t idlen = 0;
		ar >> idlen;
		sid.resize(idlen);
		ar >> sid >> score;
	}
}


