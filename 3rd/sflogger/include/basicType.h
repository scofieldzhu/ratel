#ifndef __BASIC_TYPE_H__
#define __BASIC_TYPE_H__

#include <cstdlib>
#include <cwchar>
#include <cstring>
#include <string>
#include <memory>

#ifdef _UNICODE
    #define _XChar wchar_t
    #define __X(t) L##t
    #define _xstring wstring
    #define _xCsLen wcslen
    #define _xsprintf swprintf_s
    #define _xvsprintf_s vswprintf_s
    #define _xvscprintf _vscwprintf 
    #define __XFUNCTION__  __FUNCTIONW__ 
    #define __XFILE__ __FILEW__
    #define _xgetcwd _wgetcwd
    #define _xcout wcout
    #define __xsplitpath_s _wsplitpath_s
    #define _xfopen _wfopen_s
#else
    #define _XChar char
    #define __X(t) t
    #define _xstring string
    #define _xCsLen strlen
    #define _xsprintf sprintf_s
    #define _xvsprintf_s vsprintf_s
    #define _xvscprintf _vscprintf
    #define __XFUNCTION__  __FUNCTION__ 
    #define __XFILE__ __FILE__
    #define _xgetcwd _getcwd
    #define _xcout cout
    #define __xsplitpath_s _splitpath_s
    #define _xfopen fopen_s
#endif

#define xCharT _XChar
#define _X __X
#define xStrT _xstring
#define xCsLen _xCsLen
#define xSprintf _xsprintf
#define xVSprintf_s _xvsprintf_s
#define xvscprintf _xvscprintf
#define _XFUNCTION_ __XFUNCTION__
#define _XFILE_ __XFILE__
#define xGetcwd _xgetcwd
#define xCout _xcout
#define _xsplitpath_s __xsplitpath_s
#define xfopen _xfopen
#define NONE_STR _X("None")


#pragma warning(disable: 4251 4275)
#endif