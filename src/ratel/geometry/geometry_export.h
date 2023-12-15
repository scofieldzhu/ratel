/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.basic
Module: geometry_export.h
Create time: 2023/12/14 20:18:45
=========================================================================*/
#ifndef __geometry_export_h__
#define __geometry_export_h__

#include "ratel/basic/cross_platform.h"
#include "ratel/basic/ratel_nsp.h"

#ifdef PLATFORM_WIN
	#ifdef GEOMETRY_EXPORTS
		#define RATEL_GEOMETRY_API __declspec(dllexport)
	#else
		#define RATEL_GEOMETRY_API __declspec(dllimport)
	#endif
#endif

#endif