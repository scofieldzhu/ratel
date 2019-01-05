/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: skilledfstream.h 
CreateTime: 2018-12-30 10:33
=========================================================================*/
#ifndef __skilledfstream_h__
#define __skilledfstream_h__

#include <fstream>
#include "kernelpublic.h"

RATEL_NAMESPACE_BEGIN
class RATEL_KERNEL_API AgileFilePipe
{
public:
    enum PosType
    {
        BEGIN,
        CUR,
        END
    };
    explicit operator bool()const;
    bool operator!()const;
    bool eof()const;
    bool fail()const;    
    AgileFilePipe& eat(int32 off, uint32 size);
    AgileFilePipe& shrink(uint32 size);
    AgileFilePipe& operator<<(bool val);
    AgileFilePipe& operator<<(short val);
    AgileFilePipe& operator<<(unsigned short val);
    AgileFilePipe& operator<<(int val);
    AgileFilePipe& operator<<(unsigned int val);
    AgileFilePipe& operator<<(long val);
    AgileFilePipe& operator<<(unsigned long val);
    AgileFilePipe& operator<<(long long val);
    AgileFilePipe& operator<<(unsigned long long val);
    AgileFilePipe& operator<<(float val);
    AgileFilePipe& operator<<(double val);
    AgileFilePipe& operator<<(long double val);
    AgileFilePipe& operator<<(void* val);
    AgileFilePipe& operator<<(std::streambuf* sb);
    AgileFilePipe& operator<<(std::ostream& (*pf)(std::ostream&));
    AgileFilePipe& operator<<(std::ios& (*pf)(std::ios&));
    AgileFilePipe& operator<<(std::ios_base& (*pf)(std::ios_base&));
    AgileFilePipe& setReadPos(int32 pos, PosType);
    AgileFilePipe& readData(char* data, uint32 size);
    uint32 tellReadPos();
    AgileFilePipe& ignore(uint32 cnt = 1, int32 delim = std::char_traits<char>::eof());
    uint32 getCount()const;
    AgileFilePipe& operator>>(bool val);
    AgileFilePipe& operator>>(short val);
    AgileFilePipe& operator>>(unsigned short val);
    AgileFilePipe& operator>>(int val);
    AgileFilePipe& operator>>(unsigned int val);
    AgileFilePipe& operator>>(long val);
    AgileFilePipe& operator>>(unsigned long val);
    AgileFilePipe& operator>>(long long val);
    AgileFilePipe& operator>>(unsigned long long val);
    AgileFilePipe& operator>>(float val);
    AgileFilePipe& operator>>(double val);
    AgileFilePipe& operator>>(long double val);
    AgileFilePipe& operator>>(void* val);
    AgileFilePipe& operator>>(std::streambuf* sb);
    AgileFilePipe& operator>>(std::istream& (*pf)(std::istream&));
    AgileFilePipe& operator>>(std::ios& (*pf)(std::ios&));
    AgileFilePipe& operator>>(std::ios_base& (*pf)(std::ios_base&));
    AgileFilePipe& setWritePos(int32 pos, PosType);    
    AgileFilePipe& writeData(const char* data, uint32 size);
    uint32 tellWritePos();
    void open(const char* filepath);
    void open(const std::string& filepath);
    bool isOpened()const;
    void trunc();
    void close();
    const std::string& filepath()const{return filepath_;}
    AgileFilePipe(const char* filepath);
    AgileFilePipe(const std::string& filepath);
    ~AgileFilePipe();   

private:
    std::fstream fs_;
    std::string filepath_;
};
RATEL_NAMESPACE_END

#endif
