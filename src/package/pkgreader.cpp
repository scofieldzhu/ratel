/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: rtlpkgfile.cpp 
CreateTime: 2018-9-1 19:34
=========================================================================*/
#include "pkgreader.h"
#include "pkgfilebasic.h"
#include "pkglogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

namespace{
    const uint32_t kReadBufferSize = 1024 * 4; // 4K
}

PKGReader::PKGReader()
{}

PKGReader::~PKGReader()
{
    close();
}

bool PKGReader::loadFile(const Path& pkgfile, const Path& outputdbfile, const Path& outputdatafile)
{
	do{
		if(!open(pkgfile)){
			slog_err(pkglogger) << " open pkg file(" << pkgfile.rstring().cstr() << ") failed!" << endl;
			break;
		}
		ofstream dbofs(outputdbfile.toLocale().c_str(), ios_base::out | ios_base::binary);
		ofstream dataofs(outputdatafile.toLocale().c_str(), ios_base::out | ios_base::binary);
		if(dbofs.fail() || dataofs.fail()){
			slog_err(pkglogger) << " open db file(" << outputdbfile.rstring().cstr() << ") or data file(" << outputdatafile.cstr() << ") failed!" << endl;
			break;
		}
		if(!loadNextFileData(dbofs)){
			slog_err(pkglogger) << " load db file content failed!" << endl;
			break;
		}
		if(!loadNextFileData(dataofs)){
			slog_err(pkglogger) << " load data file content failed!" << endl;
			break;
		}
		close();
		return true;
	}while(0);	
	close();
	return false;
}

bool PKGReader::open(const Path& filepath) 
{
    if(!filepath.isRegularFile()){
        slog_err(pkglogger) << "pkg file(" << filepath.rstring().cstr() << ") isn't single file!" << endl;
        return false;
    }
    fs_.open(filepath.toLocale().c_str(), ios_base::in | ios_base::binary);        
    if(fs_)
        return readTypeId();        
    fs_.close();
    return false;    
}

bool PKGReader::loadNextFileData(std::ofstream& os)
{
	logverify(pkglogger, fs_ && os);
	uint32_t filesize = 0;
	fs_.read((char*)&filesize, sizeof(uint32_t));
    char databuffer[kReadBufferSize] = {'\0'};
    int32_t curdigestion = 0, leftover = 0, needreadbytes = 0;
    while(curdigestion < filesize){        
        leftover = filesize - curdigestion;
        if(leftover <= 0) //realdy finished!
            break;
        needreadbytes = (leftover >= kReadBufferSize ? kReadBufferSize : leftover);
        fs_.read(databuffer, needreadbytes);
        if(fs_.gcount() < needreadbytes){//some exception error            
            if(fs_.eof())
                slog_err(pkglogger) << "db file data is not integrated!" << endl;
            os.write(databuffer, fs_.gcount());
            os.flush();
            return false;
        }
        os.write(databuffer, needreadbytes);
        os.flush();
        curdigestion += needreadbytes;
    };
    return true;
}

void PKGReader::close()
{
    if(fs_.is_open())
        fs_.close();
}

bool PKGReader::readTypeId()
{
    char typebuf[kPkgTypeCharNum + 1] = { '\0' };
    fs_.read(typebuf, kPkgTypeCharNum);
    if(fs_){
        RString typestr(typebuf);
        if(typestr == PKGTYPE_CODE) 
            return true;
    }
    return false;    
}

RATEL_NAMESPACE_END