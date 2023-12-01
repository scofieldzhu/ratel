/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: ratelnsp.h 
=========================================================================*/
#ifndef __ratelnsp_h__
#define __ratelnsp_h__

#define NAMESPACE_BEGIN(name) namespace name{
#define NAMESPACE_END }
#define USING_NAMESPACE(name) using namespace name;

#define RATEL ratel
#define RATEL_NAMESPACE_BEGIN NAMESPACE_BEGIN(RATEL)
#define RATEL_NAMESPACE_END NAMESPACE_END
#define USING_RATEL USING_NAMESPACE(RATEL)

#endif
