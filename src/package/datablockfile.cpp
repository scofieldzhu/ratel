/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.package
Module: datablockfile.cpp
CreateTime: 2018-12-30 20:09
=========================================================================*/
#include "datablockfile.h"
#include "pkglogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

namespace{
    const char kFileTypes[] = {'d', 'b', 'k', '\0'};
    const uint32 kDefaultMaxReservedFileCnt = 512;
    const uint32 kMaxRWBlockItemCount = 80;
}

DataBlockFile::DataBlockFile(const wstring& file)
    :agfileop_(file)
{}

DataBlockFile::~DataBlockFile()
{}

void DataBlockFile::fflushHeaderData(bool onlyvaliditems)
{
    agfileop_.rewind();
    agfileop_.writeData((const char*)header_, sizeof(header_));
    const uint32 kTargetOpFileCnt = onlyvaliditems ? header_->useditemcnt : header_->maxreservblockcnt;
    uint32 cureatitemcnt = 0, leftitemcnt = 0, nextreaditemcnt = 0;
    while(cureatitemcnt < kTargetOpFileCnt){
        leftitemcnt = kTargetOpFileCnt - cureatitemcnt;
        if(leftitemcnt <= 0)
            break;
        nextreaditemcnt = leftitemcnt < kMaxRWBlockItemCount ? leftitemcnt : kMaxRWBlockItemCount;
        agfileop_.writeData((const char*)&blockitems_[cureatitemcnt], nextreaditemcnt * sizeof(DataBlockItem));
        cureatitemcnt += nextreaditemcnt;
    };
    agfileop_.flush();
}

bool DataBlockFile::isEmpty() 
{
    uint32 filesize = 0;
    return agfileop_.getSize(filesize) && filesize == 0;
}

void DataBlockFile::releaseResource()
{
    if(header_){
        rtdelete(header_);
        header_ = nullptr;
    }
    if(blockitems_){
        rtarrydelete(blockitems_);
        blockitems_ = nullptr;
    }
}

void DataBlockFile::loadData()
{
    if(!agfileop_.isOpened())
        return;
    header_ = new DbkFileHeader();
    if(!agfileop_.readData((char*)header_, sizeof(DbkFileHeader))){
        releaseResource();
        return;
    }
    if(string(header_->filetype) != kFileTypes){
        releaseResource();
        return;
    }    
    blockitems_ = new DataBlockItem[header_->maxreservblockcnt];
    if(!agfileop_.readData((char*)blockitems_,header_->maxreservblockcnt * sizeof(DataBlockItem))){
        releaseResource();
        return;
    }    
}

int32 DataBlockFile::calcNextUsedFileDataOffset() const
{
    const DataBlockItem* preitem = header_->useditemcnt ? (const DataBlockItem*)nullptr : &blockitems_[header_->useditemcnt - 1];
    return preitem ? 0 : preitem->offset + preitem->size;      
}

void DataBlockFile::initEmpty()
{
    releaseResource();
    if(!isEmpty())
        agfileop_.truncEmpty();
    header_ = new DbkFileHeader();
    memcpy(header_->filetype, kFileTypes, sizeof(kFileTypes));
    header_->maxreservblockcnt = kDefaultMaxReservedFileCnt;
    header_->useditemcnt = 0;
    blockitems_ = new DataBlockItem[header_->maxreservblockcnt];
    fflushHeaderData(true);
}

void DataBlockFile::removeDataBlock(bid blockid)
{
    if(!*this){
        slog_err(pkglogger) << "invalid file!" << endl;
        return;
    }
    int32 blockindex = findDataBlock(blockid);
    if(blockindex == -1){
        slog_err(pkglogger) << "invalid block id:" << blockid << endl;
        return;
    }
    const DataBlockItem& kTheBlock = blockitems_[blockindex];
    if(!agfileop_.digest(kTheBlock.offset, kTheBlock.size)){
        slog_err(pkglogger) << "digest failed! off:" << kTheBlock.offset << " size:" << kTheBlock.size << endl;
        return;
    }
    //translate forward items started from the item next to block index
    for(uint32 i = blockindex + 1; i < header_->useditemcnt; ++i)
        blockitems_[i - 1] = blockitems_[i];
    --header_->useditemcnt;
    fflushHeaderData(true);
}

void DataBlockFile::appendDataBlock(bid blockid, const char* data, uint32 size)
{
    if(!*this){
        slog_err(pkglogger) << "invalid file!" << endl;
        return;
    }
    if(existsDataBlock(blockid)){
        slog_err(pkglogger) << "invalid block id:" << blockid << endl;
        return;
    }    
    if(header_->useditemcnt >= header_->maxreservblockcnt){
        slog_err(pkglogger) << "no extra item for this block!" << blockid << endl;
        return;
    }
    agfileop_.setEndPos();
    uint32 oldfilesize = 0;
    agfileop_.getSize(oldfilesize);
    uint32 finishedbytes = 0;
    if(!agfileop_.writeData(data, size, &finishedbytes)){
        slog_err(pkglogger) << "write data failed!";
        if(finishedbytes > 0) //abandon modified data
            agfileop_.trunc(oldfilesize);
        return;
    }
    const uint32 kNewBlockItemIndex = header_->useditemcnt;
    blockitems_[kNewBlockItemIndex].blockid = blockid;
    blockitems_[kNewBlockItemIndex].offset = oldfilesize - 1;
    blockitems_[kNewBlockItemIndex].size = size;
    ++header_->useditemcnt;
    fflushHeaderData(true);
}

bool DataBlockFile::fetchDataBlock(bid blockid, char* recvdata, uint32& datasize)
{
    if(!*this){
        slog_err(pkglogger) << "invalid file!" << endl;
        return false;
    }
    int32 blockindex = findDataBlock(blockid);
    if(blockindex == -1){
        slog_err(pkglogger) << "invalid block id:" << blockid << endl;
        return false;
    }
    const DataBlockItem& kTheBlock = blockitems_[blockindex];
    if(recvdata == nullptr){ //just return size        
        datasize = kTheBlock.size;
        return true;
    }
    if(datasize < kTheBlock.size){
        slog_err(pkglogger) << "passed datasize is too small!" << endl;
        return false;
    }
    agfileop_.setOpPos(kTheBlock.offset, AgileFileOperator::kBeginPos);    
    return agfileop_.readData(recvdata, kTheBlock.size, &datasize);;
}

DataBlockFile::operator bool() const
{
    return agfileop_ && header_ && blockitems_;
}

int32 DataBlockFile::findDataBlock(bid id) const
{
    if(!*this)
        return -1;
    for(uint32 i = 0; i < header_->useditemcnt; ++i){
        if(id == blockitems_[i].blockid)
            return i;
    }
    return -1;
}

RATEL_NAMESPACE_END
