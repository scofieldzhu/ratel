/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.basic
Module: cross_platform.h
Create time: 2023/11/30 10:25:14
=========================================================================*/
#ifndef __cross_platform_h__
#define __cross_platform_h__

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_MSC_VER) || defined(WINDOW) || defined(_WINDOWS)
	#define PLATFORM_WIN
#elif defined(__linux__)
	#define PLATFORM_LINUX
#else
	#define PLATFORM_UNK
#endif

#endif