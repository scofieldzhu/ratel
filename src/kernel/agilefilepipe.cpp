/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: skilledfstream.cpp 
CreateTime: 2018-12-30 10:33
=========================================================================*/
#pragma warning(disable:4273 4251)
#include "agilefilepipe.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

namespace{
    ios_base::openmode kRWBMode = ios_base::in|ios_base::out|ios_base::binary;
    ios_base::openmode kWBTruncMode = ios_base::out | ios_base::binary | ios_base::trunc;

    ios_base::seekdir MatchSeekDir(AgileFilePipe::PosType pt)
    {
        if(pt==AgileFilePipe::BEGIN)
            return ios_base::beg;
        if(pt==AgileFilePipe::CUR)
            return ios_base::cur;
        return ios_base::end;
    }
}

AgileFilePipe::AgileFilePipe(const char* filepath)
    :fs_(filepath, kRWBMode),
    filepath_(filepath)
{}

AgileFilePipe::AgileFilePipe(const std::string& filepath)
    :fs_(filepath, kRWBMode),
    filepath_(filepath)
{}

AgileFilePipe::~AgileFilePipe()
{}

bool AgileFilePipe::eof() const
{
    return fs_.eof();
}

bool AgileFilePipe::fail() const
{
    return fs_.fail();
}

AgileFilePipe& AgileFilePipe::eat(int32 off, uint32 size)
{
    return *this;
}

AgileFilePipe::operator bool()const
{
    return (bool)fs_;
}

bool AgileFilePipe::operator!()const
{
    return !fs_;
}

AgileFilePipe& AgileFilePipe::shrink(uint32 size)
{
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(bool val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(short val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(unsigned short val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(int val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(unsigned int val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(long val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(unsigned long val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(long long val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(unsigned long long val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(float val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(double val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(long double val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(void* val)
{
    fs_ << val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(std::streambuf* sb)
{
    fs_ << sb;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(std::ostream& (*pf)(std::ostream&))
{
    fs_ << pf;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(std::ios& (*pf)(std::ios&))
{
    fs_ << pf;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator<<(std::ios_base& (*pf)(std::ios_base&))
{
    fs_ << pf;
    return *this;
}

AgileFilePipe& AgileFilePipe::rewindReadPos()
{
    return setReadPos(0, BEGIN);
}

AgileFilePipe& AgileFilePipe::setReadEndPos()
{
    return setReadPos(0, END);
}

AgileFilePipe& AgileFilePipe::setReadPos(int32 pos, PosType ptype)
{
    fs_.seekg(pos, MatchSeekDir(ptype));
    return *this;
}

AgileFilePipe& AgileFilePipe::readData(char* data, uint32 size)
{
    fs_.read(data, size);
    return *this;
}

uint32 AgileFilePipe::tellReadPos()
{
    return fs_.tellg();
}

AgileFilePipe& AgileFilePipe::ignore(uint32 cnt, int32 delim)
{
    fs_.ignore(cnt, delim);
    return *this;
}

uint32 AgileFilePipe::getReadCount() const
{
    return fs_.gcount();
}

AgileFilePipe& AgileFilePipe::writeData(const char* data, uint32 size)
{
    fs_.write(data, size);
    return *this;
}

uint32 AgileFilePipe::tellWritePos() 
{
    return fs_.tellp();
}

AgileFilePipe& AgileFilePipe::flush()
{
    fs_.flush();
    return *this;
}

void AgileFilePipe::open(const char* filepath)
{
    fs_.open(filepath);
    filepath_ = filepath;
}

void AgileFilePipe::open(const std::string& filepath)
{
    fs_.open(filepath);
    filepath_ = filepath;
}

bool AgileFilePipe::isOpened() const
{
    return fs_.is_open();
}

void AgileFilePipe::trunc()
{
    if(fs_)
        fs_.close();
    fs_.open(filepath_, kWBTruncMode);
}

void AgileFilePipe::close()
{
    fs_.close();
}

uint32 AgileFilePipe::getSize() 
{
    setReadPos(0, END);
    return tellReadPos();
}

AgileFilePipe& AgileFilePipe::operator>>(bool val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(short val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(unsigned short val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(int val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(unsigned int val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(long val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(unsigned long val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(long long val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(unsigned long long val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(float val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(double val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(long double val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(void* val)
{
    fs_ >> val;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(std::streambuf* sb)
{
    fs_ >> sb;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(std::istream& (*pf)(std::istream&))
{
    fs_ >> pf;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(std::ios& (*pf)(std::ios&))
{
    fs_ >> pf;
    return *this;
}

AgileFilePipe& AgileFilePipe::operator>>(std::ios_base& (*pf)(std::ios_base&))
{
    fs_ >> pf;
    return *this;
}

AgileFilePipe& AgileFilePipe::rewindWritePos()
{
    return setWritePos(0, BEGIN);
}

AgileFilePipe& AgileFilePipe::setWriteEndPos()
{
    return setWritePos(0, END);
}

AgileFilePipe& AgileFilePipe::setWritePos(int32 pos, PosType ptype)
{
    fs_.seekp(pos, MatchSeekDir(ptype));
    return *this;
}
RATEL_NAMESPACE_END


