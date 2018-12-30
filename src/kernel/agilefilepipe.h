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
class AgileFilePipe
{
public:
    explicit operator bool()const;
    bool operator!()const;
    bool eof()const;
    bool fail()const;    
    AgileFilePipe& eatOff(int32 off, uint32 size);
    AgileFilePipe& shrink(uint32 size);
    AgileFilePipe& setRPosToBegin(int32 off);
    AgileFilePipe& setRPosToCur(int32 off);
    AgileFilePipe& setRPosToEnd(int32 off);
    AgileFilePipe(const char* filepath, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    AgileFilePipe(const std::string& filepath, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    ~AgileFilePipe();   

private:
    std::fstream fs_;
};
RATEL_NAMESPACE_END

#endif
