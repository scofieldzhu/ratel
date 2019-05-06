/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: testcls.h 
CreateTime: 2019-4-27 21:01
=========================================================================*/
#pragma once

#include "sobject.h"

class TestCls : public RATEL::SObject
{

	DECL_SERIAL(TestCls)	

public:
	TestCls();
	~TestCls();
};
