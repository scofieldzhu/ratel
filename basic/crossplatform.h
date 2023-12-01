/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.basic
Module: crossplatform.h
=========================================================================*/
#ifndef __crossplatform_h__
#define __crossplatform_h__

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_MSC_VER) || defined(WINDOW) || defined(_WINDOWS)
	#define PLATFORM_WIN
#elif defined(__linux__)
	#define PLATFORM_LINUX
#else
	#define PLATFORM_UNK
#endif

#endif