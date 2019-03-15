/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.package
Module: datablockfile.h
CreateTime: 2018-12-30 20:09
=========================================================================*/
#ifndef __datablockfile_h__
#define __datablockfile_h__

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
class RATEL_PACKAGE_API DataBlockFile
{
public:
    using UID = std::string;
	static UID NewUID();
    void removeDataBlock(const UID& blockid);
    void appendDataBlock(const UID& blockid, const char* data, uint32_t size);
	void appendDataBlock(const UID& blockid, const Path& sourcefile);
    bool fetchDataBlock(const UID& blockid, char* recvdata, uint32_t& datasize);
    operator bool()const;
    int32_t findDataBlock(const UID& id)const;
    bool existsDataBlock(const UID& id)const{return findDataBlock(id) != -1;}
    void initEmpty();
	const std::wstring& filePath()const { return agfileop_.filePath(); }
    DataBlockFile(const std::wstring& file);
    ~DataBlockFile();

private:
    void fflushHeaderData(bool onlyvaliditems);
    bool isEmpty();
    void releaseResource();
    void loadData();
    int32_t calcNextUsedFileDataOffset()const;
    struct DataBlockItem
	{
        UID blockid;
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
