/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.package
Module: datablockstorage.h
CreateTime: 2018-12-30 20:09
=========================================================================*/
#ifndef __datablockstorage_h__
#define __datablockstorage_h__

/***********************************************************************
FileType                    // dbk 4 bytes
MaxReservedFileCount        // 4 bytes
CurrentBlockCount            // 4 bytes
BlockID0 DataOffset DataSize //4 * 3 = 12bytes
BlockID1 DataOffset DataSize //4 * 3 = 12bytes
BlockID2 DataOffset DataSize //4 * 3 = 12bytes
   :         :          :
BlockIDn DataOffset DataSize //4 * 3 = 12bytes                                
************************************************************************/
#include "packagepublic.h"
#include "agilefileoperator.h"

RATEL_NAMESPACE_BEGIN
class RATEL_PACKAGE_API DataBlockStorage
{
public:
    using UID = std::string;
	enum{
		UID_LEN = 37, 
		MAX_BLOCK_NUM = 512
	};
	static UID NewUID();
    void removeDataBlock(const UID& blockid);
    void appendDataBlock(const UID& blockid, const char* data, uint32_t size);
	void appendDataBlock(const UID& blockid, const Path& sourcefile);
    bool fetchDataBlock(const UID& blockid, char* recvdata, uint32_t& datasize);
	bool exportDataBlock(const UID& blockid, const Path& targetfile);
    operator bool()const;
    int32_t findDataBlock(const UID& id)const;
    bool existsDataBlock(const UID& id)const{return findDataBlock(id) != -1;}
    void initEmpty(uint32_t maxnumofblocks = MAX_BLOCK_NUM);
	bool load();
	const std::wstring& filePath()const { return agfileop_.filePath(); }
    DataBlockStorage(const std::wstring& file);
    ~DataBlockStorage();

private:
    void fflushHeaderData(bool onlyvaliditems);
    bool isEmpty();
    void releaseResource();    
    int32_t calcNextUsedFileDataOffset()const;
    struct DataBlockItem
	{
        char blockid[UID_LEN]; //terminator is '\0'
        uint32_t offset = 0; // from data zone start position
        uint32_t size = 0;
    };
    struct DbkFileHeader
	{
        char filetype[4]; //dbk
        uint32_t maxreservblockcnt = 0;
        uint32_t useditemcnt = 0;
    };
    struct DbkFileInfo
	{
        DbkFileHeader header;
        DataBlockItem itemarray[1]; //only placeholder
    };
    AgileFileOperator agfileop_;
    DbkFileHeader* header_ = nullptr;
    DataBlockItem* blockitems_ = nullptr;
};

RATEL_NAMESPACE_END

#endif
