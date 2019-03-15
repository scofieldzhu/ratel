/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: pkgfilewriter.h 
CreateTime: 2018-9-9 17:57
=========================================================================*/
#ifndef __pkgfilewriter_h__
#define __pkgfilewriter_h__

#include <fstream>
#include "packagepublic.h"
#include "path.h"

RATEL_NAMESPACE_BEGIN

class RATEL_PACKAGE_API PKGWriter
{
public:    	
	bool write();
    PKGWriter(const Path& filepath, const Path& dbfile, const Path& datafile);
    ~PKGWriter();

private:
	void close();
	bool prepare();    
	bool writeFileData(const Path& file);
	Path pkgfilepath_;
	Path dbfilepath_;
	Path datafilepath_;
    std::ofstream ofs_;    
};

RATEL_NAMESPACE_END
#endif
