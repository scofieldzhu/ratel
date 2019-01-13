/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: agilefileoperator.cpp 
CreateTime: 2018-12-30 10:33
=========================================================================*/
#pragma warning(disable:4273 4251)
#include "agilefileoperator.h"
#include "kernellogger.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

namespace{
    const DWORD kRWMode = GENERIC_READ | GENERIC_WRITE;
    const map<AgileFileOperator::PosType, DWORD> kMoveMethodDict = {
        {AgileFileOperator::kBeginPos, FILE_BEGIN},
        {AgileFileOperator::kCurPos, FILE_CURRENT},
        {AgileFileOperator::kEndPos, FILE_END}
    };
}

#define CHECK_OPENED()\
    if(!isOpened()){\
        slog_err(kernellogger) << "no file is opened yet!" << endl; \
        return false; \
    }

AgileFileOperator::AgileFileOperator(const wstring& filepath)
{
    open(filepath);
}

AgileFileOperator::~AgileFileOperator()
{
    close();
}

bool AgileFileOperator::open(const std::wstring& filename)
{
    if(filepath_ == filename){
        if(isOpened()) //already done!
            return true;
    }else{
        filepath_ = filename;
        close();
    } 
    fhandle_ = ::CreateFileW(filepath_.c_str(),
                            kRWMode,
                            FILE_SHARE_READ,
                            nullptr,
                            OPEN_ALWAYS, //create it if not exists!
                            FILE_ATTRIBUTE_NORMAL,
                            nullptr);
    if(fhandle_ == INVALID_HANDLE_VALUE)
        slog_err(kernellogger) << "CreateFileW failed! last err=" << ::GetLastError() << endl;
    return isOpened();
}

bool AgileFileOperator::isOpened() const
{
    return fhandle_ != INVALID_HANDLE_VALUE;
}

void AgileFileOperator::close()
{
    if(fhandle_ != INVALID_HANDLE_VALUE){
        ::CloseHandle(fhandle_);
        fhandle_ = INVALID_HANDLE_VALUE;
    }        
}

AgileFileOperator::operator bool() const
{
    return isOpened();
}

bool AgileFileOperator::digest(uint32 off, uint32 size)
{
    CHECK_OPENED();
    uint32 filesize = 0;
    getSize(filesize);
    if(off >= filesize || off + size > filesize){
        slog_err(kernellogger) << "invalid params passed! perhaps it's too large!" << endl;
        return false;
    }
    static const uint32 kBufferSize = 1024;
    char databuffer[kBufferSize] = {'\0'};    
    uint32 rpos = off + size, wpos = off;    
    while(rpos < filesize){
        uint32 opsize = rpos + kBufferSize >= filesize ? filesize - rpos : kBufferSize;
        setOpPos(rpos, kBeginPos);
        readData(databuffer, opsize);
        setOpPos(wpos, kBeginPos);
        writeData(databuffer, opsize);
        rpos += opsize;
        wpos += opsize;
    }    
    return trunc(filesize - size);
}

bool AgileFileOperator::ignore(uint32 bytecnt)
{
    CHECK_OPENED()
    setOpPos(bytecnt, kCurPos);
    return true;
}

bool AgileFileOperator::readData(char* recvdata, uint32 datasize, uint32* bytestoread)
{
    if(isOpened())
        return ::ReadFile(fhandle_, recvdata, datasize, (LPDWORD)bytestoread, nullptr);    
    return false;
}

void AgileFileOperator::rewind()
{
    if(isOpened())
        setOpPos(0, kBeginPos);
}

void AgileFileOperator::setEndPos()
{
    if(isOpened())
        setOpPos(0, kEndPos);
}

bool AgileFileOperator::setOpPos(int32 off, PosType t)
{
    CHECK_OPENED()
    LARGE_INTEGER li;
    li.QuadPart = off;
    return ::SetFilePointerEx(fhandle_, li, nullptr, kMoveMethodDict.at(t)) != 0;
}

uint32 AgileFileOperator::tellPos()
{
    CHECK_OPENED();
    LARGE_INTEGER li;
    li.QuadPart = 0;
    return SetFilePointerEx(fhandle_, li, nullptr, FILE_CURRENT);    
}

bool AgileFileOperator::writeData(const char* data, uint32 size, uint32* writtenbytenum)
{
    if(isOpened())
        return ::WriteFile(fhandle_, data, size, (LPDWORD)writtenbytenum, nullptr);
    return false;
}

bool AgileFileOperator::flush()
{
   if(!isOpened())
       return false;
    if(::FlushFileBuffers(fhandle_) == 0){
        slog_err(kernellogger) << "FlushFileBuffers failed! errcode=" << ::GetLastError() << endl;
        return false;
    }
    return true;   
}

bool AgileFileOperator::trunc(uint32 size)
{
    if(!isOpened())
        return false;
    if(!setOpPos(size - 1, kBeginPos)){
        slog_err(kernellogger) << "setPos failed!" << endl;
        return false;
    }
    if(::SetEndOfFile(fhandle_) == 0){
        slog_err(kernellogger) << "SetEndOfFile failed! errcode=" << ::GetLastError() << endl;
        return false;
    }
    return true;
}

bool AgileFileOperator::truncEmpty()
{
    return trunc(0);
}

bool AgileFileOperator::getSize(uint32& size)
{
    if(!isOpened())
        return false;
    DWORD dsize = size;
    if(::GetFileSize(fhandle_, &dsize) == 0){
        slog_err(kernellogger) << "GetFileSizeEx failed! errcode=" << ::GetLastError() << endl;
        return false;
    }
    return true;
}

RATEL_NAMESPACE_END

