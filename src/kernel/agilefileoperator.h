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
    bool digest(uint32 off, uint32 size);
    bool readData(char* recvdata, uint32 datasize, uint32* bytestoread = nullptr);
    void rewind();
    void setEndPos();
    bool setOpPos(int32 off, PosType);
    uint32 tellPos();    
    bool writeData(const char* data, uint32 size, uint32* writtenbytenum = nullptr);
    bool flush();
    bool isOpened()const;
    bool trunc(uint32 size);
    bool truncEmpty();
    bool open(const std::wstring& filename);
    void close();
    bool getSize(uint32& size);
    const std::wstring& filepath()const{ return filepath_; }
    bool ignore(uint32 bytecnt);    
    AgileFileOperator(const std::wstring& filepath);
    ~AgileFileOperator();   

private:
    HANDLE fhandle_ = INVALID_HANDLE_VALUE;
    std::wstring filepath_;
};
RATEL_NAMESPACE_END

#endif
