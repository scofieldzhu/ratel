/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testserialization 
Module: class.h 
CreateTime: 2019-6-8 15:17
=========================================================================*/
#ifndef __class_h__
#define __class_h__

#include "sobjectlist.h"

class Class : public RATEL::SObject
{
	DECL_SERIAL(Class)
public:
	void print();
	void serialize(RATEL::Archive& ar);
	Class();
	Class(const RATEL::RString& nval);
	~Class();
	RATEL::RString name = "untitled";
	RATEL::SObjectList teachers;
	RATEL::SObjectList students;
};
#endif
