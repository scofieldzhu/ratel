/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.package
Module: datablockfile.cpp
CreateTime: 2018-12-30 20:09
=========================================================================*/
#include "datablockfile.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

namespace{
    const char* kFileTypes[] = {'d', 'b', 'k', '\0'};
    const uint32 kDefaultMaxReservedFileCnt = 512;
}

DataBlockFile::DataBlockFile(const char* file)
    :filepipe_(file)
{}

DataBlockFile::DataBlockFile(const string& file)
    :filepipe_(file)
{}

DataBlockFile::~DataBlockFile()
{}

void DataBlockFile::releaseResource()
{
    if(fileheader_){
        rtdelete(fileheader_);
        fileheader_ = nullptr;
    }
    if(fileitems_){
        rtarrydelete(fileitems_);
        fileitems_ = nullptr;
    }
}

void DataBlockFile::loadData()
{
    if(filepipe_.fail())
        return;
    fileheader_ = new DbkFileHeader();
    if(!filepipe_.readData((char*)fileheader_, sizeof(DbkFileHeader))){
        releaseResource();
        return;
    }
    if(string(fileheader_->filetype) != kFileTypes){
        releaseResource();
        return;
    }    
    fileitems_ = new DataBlockItem[fileheader_->maxreservedfilecnt];
    filepipe_.readData((char*)fileitems_,fileheader_->maxreservedfilecnt * sizeof(DataBlockItem));
    if(filepipe_.fail()){
        releaseResource();
        return;
    }
}

bool DataBlockFile::isValid()const
{
    return filepipe_ && fileheader_;
}

void DataBlockFile::initEmpty()
{
    releaseResource();
    filepipe_.trunc();
    fileheader_ = new DbkFileHeader();
    memcpy(fileheader_->filetype, kFileTypes, sizeof(kFileTypes));
    fileheader_->maxreservedfilecnt = kDefaultMaxReservedFileCnt;
    fileheader_->curfilecnt = 0;
    fileitems_ = new DataBlockItem[fileheader_->maxreservedfilecnt];
}

void DataBlockFile::removeDataBlock(fid blockid)
{

}

void DataBlockFile::appendDataBlock(fid blockid, const char* data, uint32 size)
{

}

int32 DataBlockFile::getDataBlockSize(fid id) const
{
    return -1;
}

bool DataBlockFile::fetchDataBlock(fid blockid, char* recvdata, uint32 datasize)
{
    return false;
}

RATEL_NAMESPACE_END
