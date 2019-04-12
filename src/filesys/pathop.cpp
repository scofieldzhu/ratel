/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: pathop.cpp 
CreateTime: 2019-3-20 21:18
=========================================================================*/
#include "pathop.h"
#include <shellapi.h>

RATEL_NAMESPACE_BEGIN

bool CreateDir(const Path& dirpath)
{
	return ::CreateDirectory(dirpath.toWString().c_str(), nullptr);
}

bool RemoveDir(const Path& dirpath)
{
	std::wstring dirstr = dirpath.toWString();
	wchar_t* pathbuf = new wchar_t[dirstr.size() + 2];
	wmemcpy(pathbuf, dirstr.c_str(), dirstr.size());
	pathbuf[dirstr.size()] = L'\0';
	pathbuf[dirstr.size() + 1] = L'\0';
	SHFILEOPSTRUCT fop = {
		NULL,
		FO_DELETE,
		pathbuf,
		NULL,
		FOF_NOCONFIRMATION |
		FOF_NOERRORUI |
		FOF_SILENT,
		FALSE,
		NULL,
		NULL};
	SHFileOperation(&fop);
	delete[] pathbuf;
	return fop.fAnyOperationsAborted == FALSE;
}

bool RemoveFile(const Path& path)
{
	return RemoveDir(path);
}

RATEL_NAMESPACE_END


