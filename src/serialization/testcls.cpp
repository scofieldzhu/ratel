/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: testcls.cpp 
CreateTime: 2019-4-27 21:09
=========================================================================*/
#include "testcls.h"
#include "archive.h"
USING_RATEL

IMPL_SERIAL(TestCls, SObject, 0)

TestCls::TestCls(){

}

TestCls::~TestCls(){

}
