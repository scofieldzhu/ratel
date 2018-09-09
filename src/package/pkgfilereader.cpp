/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: rtlpkgfile.cpp 
CreateTime: 2018-9-1 19:34
=========================================================================*/
#include "pkgfilereader.h"
#include "pkgfilebasic.h"
#include "packagelogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

PkgFileReader::PkgFileReader(const Path& filepath)
    :pkgpath_(filepath)
{}

PkgFileReader::~PkgFileReader()
{
    close();
}

bool PkgFileReader::open() 
{
    dbdatasize_ = -1;
    if(pkgpath_.isRegularFile()){
        slog_err(packagelogger) << "pkgpath(" << pkgpath_.toRString().cstr() << ") is invalid!" << endl;
        return false;
    }
    fs_.open(pkgpath_.toRString().cstr(), ios_base::in | ios_base::binary);        
    if(fs_){
        if(!readTypeId())
            return false;
        if (!readDBDataSize())
            return false;        
        return true;
    }
    fs_.close();
    return false;    
}

bool PkgFileReader::readDBFileData(char*& filedata, uint32& datasize)
{
    if(dbdatasize_ > 0 && fs_){        
        fs_.seekg(ios_base::beg, kPkgTypeCharNum + sizeof(uint32));
        if(fs_){
            fs_.read(filedata, dbdatasize_);
            if(fs_){
                datasize = dbdatasize_;
                return true;
            }
        }
    }            
    return false;
}

bool PkgFileReader::readFileData(int32 offset, uint32 size, char*& outdata)
{
    if(dbdatasize_ > 0 && fs_){
        fs_.seekg(ios_base::beg, kPkgTypeCharNum + sizeof(uint32) + offset);
        if(fs_){
            fs_.read(outdata, size);
            if(fs_)
                return true;
        }
    }    
    return false;
}

void PkgFileReader::close()
{
    if(fs_.is_open())
        fs_.close();
}

bool PkgFileReader::readTypeId()
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

bool PkgFileReader::readDBDataSize()
{    
    fs_.read((char*)&dbdatasize_, sizeof(uint32));
    return (fs_) ? true : false;    
}

RATEL_NAMESPACE_END