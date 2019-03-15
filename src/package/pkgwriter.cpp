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
PKGWriter::PKGWriter(const Path& pkgfile, const Path& dbfile, const Path& datafile)
    :pkgfilepath_(pkgfile),
	dbfilepath_(dbfile),
	datafilepath_(datafile)
{}

PKGWriter::~PKGWriter()
{
	close();
}

void PKGWriter::close()
{
	if(ofs_.is_open())
		ofs_.close();
}

bool PKGWriter::prepare()
{
    if(!pkgfilepath_.parentPath().exists()){
        slog_err(pkglogger) << "invalid filepath(" << pkgfilepath_.rstring().cstr() << ")!" << endl;
        return false;
    }
    ofs_.open(pkgfilepath_.rstring().cstr(), ios_base::binary | ios_base::out);
    if(!ofs_.is_open()){
        slog_err(pkglogger) << "open filepath(" << pkgfilepath_.rstring().cstr() << ") failed for write!" << endl;
        return false;
    }
    ofs_.write(PKGTYPE_CODE, strlen(PKGTYPE_CODE));
    return true;    
}

bool PKGWriter::writeFileData(const Path& thefile)
{
    if(!thefile.exists() || !thefile.isRegularFile()) {
        slog_err(pkglogger) << "invalid filepath(" << thefile.rstring().cstr() << ")!" << endl;
        return false;
    }
    ifstream ifs_(thefile.rstring().cstr(), ios_base::in | ios_base::binary);
    if(!ifs_.is_open()){
        slog_err(pkglogger) << "open dbfile(" << thefile.rstring().cstr() << ") failed for read!" << endl;
        return false;
    }
    //write filesize field 
    ifs_.seekg(0, ios_base::end);
    std::streampos filesize = ifs_.tellg();
    ifs_.seekg(0, ios_base::beg);
    ofs_.write((const char*)&filesize, sizeof(uint32_t));
    //write file content
    const uint32_t kBufSize = 512;
    char buffer[kBufSize] = {'\0'};
    while(true){
        if(ifs_.read(buffer, kBufSize)){
            ofs_.write(buffer, kBufSize);
            continue;
        }
        std::streamsize actualreadbytes = ifs_.gcount();
        if(actualreadbytes > 0)
            ofs_.write(buffer, actualreadbytes);
        break;
    }
    ifs_.close();
    return true;
}

bool PKGWriter::write()
{
	do{
		if(!prepare()){
			slog_err(pkglogger) << " prepare failed!" << endl;
			break;
		}
		if(!writeFileData(dbfilepath_)){
			slog_err(pkglogger) << " write db file content failed!" << endl;
			break;
		}
		if(!writeFileData(datafilepath_)){
			slog_err(pkglogger) << " write data file content failed!" << endl;
			break;
		}
		close();	
		return true;
	}while(0);	
	close();	
	return false;
}

RATEL_NAMESPACE_END
