/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: rtlpkgfile.h 
CreateTime: 2018-9-1 14:25
=========================================================================*/
#ifndef __rtlpkgfile_h__
#define __rtlpkgfile_h__

#include <fstream>
#include "packagepublic.h"
#include "path.h"

RATEL_NAMESPACE_BEGIN

class RATEL_PACKAGE_API PKGReader
{
public:
	bool load(const Path& pkgfile, const Path& targetdir);
    PKGReader();
    ~PKGReader();

private:
	void close();
	bool open(const Path& pkgfile);	
	bool loadNextFile(const Path& targetdir);	
    bool readTypeId();
    std::ifstream fs_;
};
RATEL_NAMESPACE_END
#endif
