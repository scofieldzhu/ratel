/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testserialization 
Module: teacher.h 
CreateTime: 2019-6-7 21:50
=========================================================================*/
#ifndef __teacher_h__
#define __teacher_h__

#include "person.h"

class Teacher : public Person
{
	DECL_SERIAL(Teacher)
public:
	void print();
	void serialize(RATEL::Archive& ar);
	Teacher();
	Teacher(const RATEL::RString&);
	RATEL::RString tid;
	double salary = 0.0;
};

#endif
