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
CurrentFileCount            // 4 bytes
FileID0 DataOffset DataSize //4 * 3 = 12bytes
FileID1 DataOffset DataSize //4 * 3 = 12bytes
FileID2 DataOffset DataSize //4 * 3 = 12bytes
   :         :          :
FileIDn DataOffset DataSize //4 * 3 = 12bytes                                
************************************************************************/

#include "packagepublic.h"
#include "agilefilepipe.h"

RATEL_NAMESPACE_BEGIN
class RATEL_PACKAGE_API DataBlockFile
{
public:
    using fid = uint32;
    void removeDataBlock(fid blockid);
    void appendDataBlock(fid blockid, const char* data, uint32 size);
    int32 getDataBlockSize(fid id)const;
    bool fetchDataBlock(fid blockid, char* recvdata, uint32 datasize);
    bool isValid()const;
    void initEmpty();
    DataBlockFile(const char* file);
    DataBlockFile(const std::string& file);
    ~DataBlockFile();

private:
    void fflushHeaderData();
    bool isEmpty();
    void releaseResource();
    void loadData();
    enum DBlockSate{
        DBLOCKSATE_EMPTY,
        DBLOCKSATE_NORMAL,
        DBLOCKSATE_FAKE
    };
    struct DataBlockItem{
        fid fileid = 0;
        uint32 offset = 0; // from data zone start position
        uint32 size = 0;
        DBlockSate state = DBLOCKSATE_EMPTY;
    };
    struct DbkFileHeader{
        char filetype[4]; //dbk
        uint32 maxreservedfilecnt = 0;
        uint32 maxuseditemindex = 0;
    };
    struct DbkFileInfo{
        DbkFileHeader header;
        DataBlockItem fileitem[1]; //only placeholder
    };
    AgileFilePipe filepipe_;
    DbkFileHeader* fileheader_ = nullptr;
    DataBlockItem* fileitems_ = nullptr;
};

void CreateDataBlockFile();

RATEL_NAMESPACE_END

#endif
