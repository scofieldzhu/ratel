/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: path.h 
CreateTime: 2018-6-9 20:23
=========================================================================*/
#ifndef __path_h__
#define __path_h__

#include "filesyspublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_FILESYS_API Path
{
public:
    static const char Separator;
    const char* cstr()const { return pathstr_.cstr(); }
    const RString& rstring()const { return pathstr_; }
    bool exists()const;
    bool isDirectory()const;
    bool isRegularFile()const;
    Path join(const Path& rhs)const;
    Path parentPath()const;
    Path filename()const;
    bool null()const { return pathstr_.null(); }
    bool empty()const { return pathstr_.empty(); }
    Path();
    Path(const char* str);
    Path(const wchar_t* wstr);
    Path(const RString& str);
    ~Path();

private:
    size_t findLastSeparator()const;
    RString pathstr_;
};
RATEL_NAMESPACE_END

#endif
