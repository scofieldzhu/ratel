/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: agilefileoperator.h 
CreateTime: 2018-12-30 10:33
=========================================================================*/
#ifndef __agilefileoperator_h__
#define __agilefileoperator_h__

#include "kernelpublic.h"

RATEL_NAMESPACE_BEGIN
class RATEL_KERNEL_API AgileFileOperator
{
public:
    enum PosType{
        kBeginPos,
        kCurPos,
        kEndPos
    };    
    operator bool()const;
    bool digest(uint32_t off, uint32_t size);
    bool readData(char* recvdata, uint32_t datasize, uint32_t* bytestoread = nullptr);
	char* readWholeData(uint32_t& datasize); //returned data is not free unless you use 'delete[]' operator to destroy it explicitly!
    void rewind();
    void setEndPos();
    bool setOpPos(int32_t off, PosType);
    uint32_t tellPos();    
    bool writeData(const char* data, uint32_t size, uint32_t* writtenbytenum = nullptr);
    bool flush();
    bool isOpened()const;
    bool trunc(uint32_t size);
    bool truncEmpty();
    bool open(const std::wstring& filename);
    void close();
    bool getSize(uint32_t& size);
    const std::wstring& filePath()const{ return filepath_; }
    bool ignore(uint32_t bytecnt);    
    AgileFileOperator(const std::wstring& filepath);
    ~AgileFileOperator();   

private:
    HANDLE fhandle_ = INVALID_HANDLE_VALUE;
    std::wstring filepath_;
};
RATEL_NAMESPACE_END

#endif
