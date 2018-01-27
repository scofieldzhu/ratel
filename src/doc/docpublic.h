/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.doc 
Module: docpublic.h 
=========================================================================*/
#ifndef __doc_h__
#define __doc_h__

#ifdef DOC_EXPORTS
    #define RATEL_DOC_API __declspec(dllexport)
#else
    #define RATEL_DOC_API __declspec(dllimport)
#endif

#endif
