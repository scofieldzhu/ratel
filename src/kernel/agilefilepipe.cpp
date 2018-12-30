/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: skilledfstream.cpp 
CreateTime: 2018-12-30 10:33
=========================================================================*/
#include "agilefilepipe.h"

RATEL_NAMESPACE_BEGIN

AgileFilePipe::AgileFilePipe(const char* filepath, std::ios_base::openmode mode)
    :fs_(filepath, mode)
{}

AgileFilePipe::AgileFilePipe(const std::string& filepath, std::ios_base::openmode mode)
    :fs_(filepath, mode)
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

AgileFilePipe::operator bool()const
{
    return (bool)fs_;
}

bool AgileFilePipe::operator!()const
{
    return !fs_;
}

AgileFilePipe& AgileFilePipe::eatOff(int32 off, uint32 size)
{

}

AgileFilePipe& AgileFilePipe::shrink(uint32 size)
{

}

AgileFilePipe& AgileFilePipe::setRPosToBegin(int32 off)
{
    fs_.see
}

AgileFilePipe& AgileFilePipe::setRPosToCur(int32 off)
{

}

AgileFilePipe& AgileFilePipe::setRPosToEnd(int32 off)
{

}

RATEL_NAMESPACE_END