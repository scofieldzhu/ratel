/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: rtlpkgfile.cpp 
CreateTime: 2018-9-1 19:34
=========================================================================*/
#include "pkgreader.h"
#include "fsutil.h"
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

bool PKGReader::load(const Path& pkgfile, const Path& targetdir, Path& dbfile)
{
	if(!targetdir.isDirectory()){
		slog_err(pkglogger) << "targetdir(" << targetdir.cstr() << ") not exists!" << endl;
		return false;
	}
	if(!open(pkgfile)){
		slog_err(pkglogger) << " open pkg file(" << pkgfile.rstring().cstr() << ") failed!" << endl;
		return false;
	}
	RString fn;
	if(!loadNextFile(targetdir, fn)){ //first file is database file				
		slog_err(pkglogger) << " load db file failed!" << endl;
		close();
		return false;
	}
	dbfile = fn;
	while(loadNextFile(targetdir, fn))
		;
	close();
	return true;
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

bool PKGReader::loadNextFile(const Path& targetdir, RString& outfilename)
{
	if(fs_.eof())
		return false;

	//read filename string
	streamsize bytestoread = 0;
	fs_.read((char*)&bytestoread, sizeof(bytestoread));
	if(bytestoread > 300){
		slog_err(pkglogger) << "too large char count of filename!" << endl;
		return false;
	}
	char* fnbuf = new char[bytestoread + 1];
	fs_.read(fnbuf, bytestoread);
	fnbuf[bytestoread] = '\0';
	outfilename = fnbuf;
	delete[] fnbuf;
	
	//read file content size
	fs_.read((char*)&bytestoread, sizeof(bytestoread));

	//read file content
	Path filepath = targetdir.join(outfilename);
	ofstream ofs(filepath.toLocale().c_str(), ios_base::out | ios_base::binary);
    return fsutil::TransformDataBlock(fs_, ofs, static_cast<uint32_t>(bytestoread), kReadBufferSize);
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