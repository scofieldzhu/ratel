/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.package
Module: datablockstorage.cpp
CreateTime: 2018-12-30 20:09
=========================================================================*/
#include "datablockstorage.h"
#include "path.h"
#include "pkglogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

namespace{
    const char kFileTypes[] = {'d', 'b', 'k', '\0'};
    const uint32_t kDefaultMaxReservedFileCnt = 512;
    const uint32_t kMaxRWBlockItemCount = 80;
}

DataBlockStorage::DataBlockStorage(const wstring& file)
    :agfileop_(file)
{}

DataBlockStorage::~DataBlockStorage()
{
	releaseResource();
}

void DataBlockStorage::fflushHeaderData(bool onlyvaliditems)
{
    agfileop_.rewind();
    agfileop_.writeData((const char*)header_, sizeof(DbkFileHeader));
    const uint32_t kTargetOpFileCnt = onlyvaliditems ? header_->useditemcnt : header_->maxreservblockcnt;
    uint32_t cureatitemcnt = 0, leftitemcnt = 0, nextreaditemcnt = 0;
    while(cureatitemcnt < kTargetOpFileCnt){
        nextreaditemcnt = leftitemcnt < kMaxRWBlockItemCount ? (kTargetOpFileCnt - cureatitemcnt) : kMaxRWBlockItemCount;
        agfileop_.writeData((const char*)&blockitems_[cureatitemcnt], nextreaditemcnt * sizeof(DataBlockItem));
        cureatitemcnt += nextreaditemcnt;
    };
    agfileop_.flush();
}

bool DataBlockStorage::isEmpty() 
{
    uint32_t filesize = 0;
    return agfileop_.getSize(filesize) && filesize == 0;
}

void DataBlockStorage::releaseResource()
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

bool DataBlockStorage::load()
{
	do{
		if(!agfileop_.isOpened())
			break;
		header_ = new DbkFileHeader();
		if(!agfileop_.readData((char*)header_, sizeof(DbkFileHeader)))
			break;
		if(string(header_->filetype) != kFileTypes)
			break;
		blockitems_ = new DataBlockItem[header_->maxreservblockcnt];
		if(!agfileop_.readData((char*)blockitems_, header_->maxreservblockcnt * sizeof(DataBlockItem)))
			break;
		return true;
	}while(0);
	releaseResource();
	return false;
}

int32_t DataBlockStorage::calcNextUsedFileDataOffset() const
{
    const DataBlockItem* preitem = header_->useditemcnt ? (const DataBlockItem*)nullptr : &blockitems_[header_->useditemcnt - 1];
    return preitem ? 0 : preitem->offset + preitem->size;      
}

void DataBlockStorage::initEmpty()
{
    releaseResource();
    if(!isEmpty())
        agfileop_.truncEmpty();
    header_ = new DbkFileHeader();
    memcpy(header_->filetype, kFileTypes, sizeof(kFileTypes));
    header_->maxreservblockcnt = kDefaultMaxReservedFileCnt;
    header_->useditemcnt = 0;
    blockitems_ = new DataBlockItem[header_->maxreservblockcnt];
    fflushHeaderData(false);
}

DataBlockStorage::UID DataBlockStorage::NewUID()
{
	return RString::NewUID().cstr();
}

void DataBlockStorage::removeDataBlock(const UID& blockid)
{
    if(!*this){
        slog_err(pkglogger) << "invalid file!" << endl;
        return;
    }
    int32_t blockindex = findDataBlock(blockid);
    if(blockindex == -1){
        slog_err(pkglogger) << "invalid block id:" << blockid.c_str() << endl;
        return;
    }
    const DataBlockItem& kTheBlock = blockitems_[blockindex];
    if(!agfileop_.digest(kTheBlock.offset, kTheBlock.size)){
        slog_err(pkglogger) << "digest failed! off:" << kTheBlock.offset << " size:" << kTheBlock.size << endl;
        return;
    }
    //translate forward items started from the item next to block index
    for(uint32_t i = blockindex + 1; i < header_->useditemcnt; ++i)
        blockitems_[i - 1] = blockitems_[i];
    --header_->useditemcnt;
    fflushHeaderData(true);
}

void DataBlockStorage::appendDataBlock(const UID& blockid, const char* data, uint32_t size)
{
    if(!*this){
        slog_err(pkglogger) << "invalid file!" << endl;
        return;
    }
    if(existsDataBlock(blockid)){
        slog_err(pkglogger) << "invalid block id:" << blockid.c_str() << endl;
        return;
    }    
    if(header_->useditemcnt >= header_->maxreservblockcnt){
        slog_err(pkglogger) << "no extra item for this block!" << blockid.c_str() << endl;
        return;
    }
    agfileop_.setEndPos();
    uint32_t oldfilesize = 0;
    agfileop_.getSize(oldfilesize);
    uint32_t finishedbytes = 0;
    if(!agfileop_.writeData(data, size, &finishedbytes)){
        slog_err(pkglogger) << "write data failed!";
        if(finishedbytes > 0) //abandon modified data
            agfileop_.trunc(oldfilesize);
        return;
    }
    const uint32_t kNewBlockItemIndex = header_->useditemcnt;
	memcpy(blockitems_[kNewBlockItemIndex].blockid, blockid.c_str(), UID_LEN);
    blockitems_[kNewBlockItemIndex].offset = oldfilesize;
    blockitems_[kNewBlockItemIndex].size = size;
    ++header_->useditemcnt;
    fflushHeaderData(true);
}

void DataBlockStorage::appendDataBlock(const UID& blockid, const Path& sourcefile)
{

}

bool DataBlockStorage::fetchDataBlock(const UID& blockid, char* recvdata, uint32_t& datasize)
{
    if(!*this){
        slog_err(pkglogger) << "invalid file!" << endl;
        return false;
    }
    int32_t blockindex = findDataBlock(blockid);
    if(blockindex == -1){
        slog_err(pkglogger) << "invalid block id:" << blockid.c_str() << endl;
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

bool DataBlockStorage::exportDataBlock(const UID& blockid, const Path& targetfile)
{
	if(!*this){
		slog_err(pkglogger) << "invalid file!" << endl;
		return false;
	}
	HANDLE outfh = ::CreateFileW(targetfile.toWString().c_str(),
								GENERIC_WRITE,
								0,
								nullptr,
								CREATE_ALWAYS, 
								FILE_ATTRIBUTE_NORMAL,
								nullptr);
	if(outfh == INVALID_HANDLE_VALUE){
		slog_err(pkglogger) << "CreateFileW(" << targetfile.toLocale().c_str() <<") lasterror= " << ::GetLastError() <<"." << endl;
		return false;
	}
	int32_t blockindex = findDataBlock(blockid);
	if(blockindex == -1){
		::CloseHandle(outfh);
		slog_err(pkglogger) << "invalid block id:" << blockid.c_str() << endl;
		return false;
	}
	const DataBlockItem& kTheBlock = blockitems_[blockindex];
	agfileop_.setOpPos(kTheBlock.offset, AgileFileOperator::kBeginPos);
	bool result = agfileop_.readDataBlock(outfh, kTheBlock.size);
	::CloseHandle(outfh);
	return result;
}

DataBlockStorage::operator bool() const
{
    return agfileop_ && header_ && blockitems_;
}

int32_t DataBlockStorage::findDataBlock(const UID& id) const
{
    if(!*this)
        return -1;
    for(uint32_t i = 0; i < header_->useditemcnt; ++i){
        if(id == blockitems_[i].blockid)
            return i;
    }
    return -1;
}

RATEL_NAMESPACE_END
