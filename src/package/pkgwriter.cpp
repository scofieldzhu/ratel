/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: pkgfilewriter.cpp 
CreateTime: 2018-9-9 17:57
=========================================================================*/
#include "pkgwriter.h"
#include "pkgfilebasic.h"
#include "pkglogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN
PKGWriter::PKGWriter()
{}

PKGWriter::~PKGWriter()
{
	destroy();
}

bool PKGWriter::reInit(const Path& targetpkgfile, const Path& dbfile)
{
	if(initialized()){
		slog_err(pkglogger) << "writer was already initialized!" << endl;
		return false;
	}
	if(!open(targetpkgfile)){
		slog_err(pkglogger) << "open targetpkgfile[" << targetpkgfile.cstr() <<"] for writing failed!" << endl;
		return false;
	}
	if(!writeDataFile(dbfile)){
		destroy();
		slog_err(pkglogger) << "write db file[" << dbfile.cstr() <<"] failed!" << endl;
		return false;
	}
	return true;
}

void PKGWriter::destroy()
{
	if(ofs_.is_open())
		ofs_.close();
}

bool PKGWriter::open(const Path& pkgfile)
{
    if(!pkgfile.parentPath().exists()){
        slog_err(pkglogger) << "invalid pkg file(" << pkgfile.rstring().cstr() << ")!" << endl;
        return false;
    }
    ofs_.open(pkgfile.rstring().cstr(), ios_base::binary | ios_base::out);
    if(!ofs_.is_open()){
        slog_err(pkglogger) << "open filepath(" << pkgfile.rstring().cstr() << ") failed for writing!" << endl;
        return false;
    }
    ofs_.write(PKGTYPE_CODE, strlen(PKGTYPE_CODE));
    return true;    
}

bool PKGWriter::writeDataFile(const Path& thefile)
{
	if(!initialized()){
		slog_err(pkglogger) << "writer is uninitialized now!" << endl;
		return false;
	}
    if(!thefile.exists() || !thefile.isRegularFile()) {
        slog_err(pkglogger) << "invalid filepath(" << thefile.rstring().cstr() << ")!" << endl;
        return false;
    }	
    ifstream ifs_(thefile.rstring().cstr(), ios_base::in | ios_base::binary);
    if(!ifs_.is_open()){
        slog_err(pkglogger) << "open dbfile(" << thefile.rstring().cstr() << ") failed for read!" << endl;
        return false;
    }
	//writer file name string
	const Path fn = thefile.filename();
	streamsize charcnt = fn.rstring().size();
	ofs_.write((const char*)&charcnt, sizeof(charcnt));
	ofs_.write(fn.cstr(), charcnt);
    //write filesize field 
    ifs_.seekg(0, ios_base::end);
	charcnt = ifs_.tellg();
    ifs_.seekg(0, ios_base::beg);
    ofs_.write((const char*)&charcnt, sizeof(charcnt));
    //write file content
    const uint32_t kBufSize = 512;
    char buffer[kBufSize] = {'\0'};
    while(true){
        if(ifs_.read(buffer, kBufSize)){
            ofs_.write(buffer, kBufSize);
            continue;
        }
		streamsize actualreadbytes = ifs_.gcount();
        if(actualreadbytes > 0)
            ofs_.write(buffer, actualreadbytes);
        break;
    }
    ifs_.close();
    return true;
}

RATEL_NAMESPACE_END
