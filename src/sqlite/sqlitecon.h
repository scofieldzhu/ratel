/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: sqlitecon.h 
CreateTime: 2019-11-3 21:06
=======================================================================*/
#ifndef __sqlitecon_h__
#define __sqlitecon_h__

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API SqliteCon
{
public:
    static void InitWinTempDir(const Path& tmpdir); //make sure call this for only once.
    bool open(const Path& dbfile, int32_t flags, const char* zvfs = nullptr);
    void* handle() { return handle_; }
    bool isOpened()const { return handle_ != nullptr; }
    bool isValid()const { return handle_ != nullptr; }
    void close();
    bool isClosed()const { return !isOpened(); }
    RString lastErrMsg()const;
    SqliteCon();
    ~SqliteCon();

private:
    void* handle_ = nullptr;
};

RATEL_NAMESPACE_END

#endif
