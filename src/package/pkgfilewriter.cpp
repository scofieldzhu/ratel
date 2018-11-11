/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: pkgfilewriter.cpp 
CreateTime: 2018-9-9 17:57
=========================================================================*/
#include "pkgfilewriter.h"
#include "pkgfilebasic.h"
#include "pkglogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN
PkgFileWriter::PkgFileWriter(const Path& filepath)
    :filepath_(filepath)
{}

PkgFileWriter::~PkgFileWriter()
{
    end();
}

bool PkgFileWriter::beginWrite()
{
    if(!filepath_.exists() || !filepath_.isRegularFile()){
        slog_err(pkglogger) << "invalid filepath(" << filepath_.rstring().cstr() << ")!" << endl;
        return false;
    }
    ofs_.open(filepath_.rstring().cstr(), ios_base::binary | ios_base::out);
    if(!ofs_.is_open()){
        slog_err(pkglogger) << "open filepath(" << filepath_.rstring().cstr() << ") failed for write!" << endl;
        return false;
    }
    ofs_.write(PKGTYPE_CODE, strlen(PKGTYPE_CODE));
    return true;    
}

bool PkgFileWriter::writeFileData(const Path& thefile)
{
    if(inWritting()) {
        slog_err(pkglogger) << "writer is not in writting mode at present!" << endl;
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
    //write filesize field 
    ifs_.seekg(0, ios_base::end);
    uint32 filesize = ifs_.tellg();
    ifs_.seekg(0, ios_base::beg);
    ofs_.write((const char*)&filesize, sizeof(uint32));
    //write file content
    const uint32 kBufSize = 512;
    char buffer[kBufSize] = { '\0' };
    while(true){
        if(ifs_.read(buffer, kBufSize)){
            ofs_.write(buffer, kBufSize);
            continue;
        }
        uint32 actualreadbytes = ifs_.gcount();
        if(actualreadbytes > 0)
            ofs_.write(buffer, actualreadbytes);
        break;
    }
    ifs_.close();
    return true;
}

void PkgFileWriter::end()
{
    if(inWritting())
        ofs_.close();
}

bool PkgFileWriter::inWritting() const
{
    return ofs_.is_open();
}

RATEL_NAMESPACE_END
