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
#include "pkglogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

namespace{
    const uint32_t kReadBufferSize = 1024 * 4; // 4K
}

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
    if(!pkgpath_.isRegularFile()){
        slog_err(pkglogger) << "pkg file(" << pkgpath_.rstring().cstr() << ") isn't single file!" << endl;
        return false;
    }
    fs_.open(pkgpath_.toLocale().c_str(), ios_base::in | ios_base::binary);        
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

bool PkgFileReader::readDBFileData(char*& filedata, uint32_t& datasize)
{
    if(dbdatasize_ > 0 && fs_){        
        locateDBDataPos();
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

bool PkgFileReader::readDBDataToFile(std::ofstream& os)
{
    if(!fs_ || dbdatasize_ <= 0)
        return false;
    locateDBDataPos();
    if(!fs_)
        return false;
    char databuffer[kReadBufferSize] = {'\0'};
    int32_t curdigestion = 0, leftover = 0, needreadbytes = 0;
    while(curdigestion < dbdatasize_){        
        leftover = dbdatasize_ - curdigestion;
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

bool PkgFileReader::readFileData(int32_t offset, uint32_t size, char*& outdata)
{
    if(dbdatasize_ > 0 && fs_){
        fs_.seekg(kPkgTypeCharNum + sizeof(uint32_t) + offset, ios_base::beg);
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

void PkgFileReader::locateDBDataPos()
{
    fs_.seekg(kPkgTypeCharNum + sizeof(uint32_t), ios_base::beg);
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
    fs_.read((char*)&dbdatasize_, sizeof(uint32_t));
    return (bool)fs_;    
}

RATEL_NAMESPACE_END