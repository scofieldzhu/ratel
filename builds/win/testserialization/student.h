/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testserialization 
Module: student.h 
CreateTime: 2019-6-7 19:21
=========================================================================*/
#ifndef __student_h__
#define __student_h__

#include "person.h"

class Student : public Person
{
	DECL_SERIAL(Student)
public:
	void print();
	void serialize(RATEL::Archive& ar);
	Student();
	Student(const RATEL::RString&);	
	~Student();
	RATEL::RString sid = "untitled";
	double score = 0.0;
};

#endif
