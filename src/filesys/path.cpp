/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: path.cpp 
CreateTime: 2018-6-9 20:24
=========================================================================*/
#include "path.h"

RATEL_NAMESPACE_BEGIN
const char Path::Separator = '/';

Path::Path()
{}

Path::Path(const RString& str)
    :pathstr_(str)
{}

Path::Path(const char* str)
    :pathstr_(str)
{}

Path::Path(const wchar_t* wstr)
    :pathstr_(wstr)
{}

Path::~Path()
{}

size_t Path::findLastSeparator()const
{
    size_t pos = pathstr_.findLastOf('/');
    return pos != RString::npos ? pos : pathstr_.findLastOf('\\');
}

Path& Path::join(const Path& rhs)
{
    const RString& rhsstr = rhs.rstring();
    if(rhsstr.empty())
        return *this;
    if(pathstr_.back() != '\\' && pathstr_.back() != '/')
        pathstr_.append(1, '/');
    if(rhsstr[0] == '\\' || rhsstr[0] == '/')
        pathstr_.append(rhsstr, 1, RString::npos);
    else
        pathstr_.append(rhsstr);
    return *this;
}

Path Path::join(const Path& rhs) const
{
    Path res(*this);
    res = rhs;
    return res;
}

Path Path::parentPath() const
{
    size_t pos = findLastSeparator();
    if(pos == RString::npos)
        return Path();
    return pathstr_.substr(0, pos);
}

Path Path::filename() const
{
    size_t pos = findLastSeparator();
    if(pos == RString::npos || pos == pathstr_.size() - 1)
        return *this;
    return pathstr_.substr(pos + 1);
}

bool Path::exists() const
{
    struct stat s;
    return stat(pathstr_.cstr(), &s) == 0;
}

bool Path::isDirectory() const
{
    struct stat s;
    return stat(pathstr_.cstr(), &s) == 0 && (s.st_mode & S_IFDIR);    
}

bool Path::isRegularFile() const
{
    struct stat s;
    return stat(pathstr_.cstr(), &s) == 0 && (s.st_mode & S_IFREG);
}

RATEL_NAMESPACE_END
