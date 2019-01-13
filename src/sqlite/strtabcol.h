/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: strtabcol.h 
CreateTime: 2019-1-13 20:54
=========================================================================*/
#ifndef __strtabcol_h__
#define __strtabcol_h__

#include "dbtablecol.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API StrTabCol : public DbTableCol
{
public:
    virtual const char* dataTypeString()const;
    virtual DbTableCol* clone()const;
    virtual RString description()const;
    void setValue(const RString& val){value_ = val;}
    const RString& value()const{return value_;}
    StrTabCol& setMaxCharNum(int32 maxnum){maxcharnum_ = maxnum; return *this;}
    int32 maxCharNum()const{return maxcharnum_;}
    void setDefaultValue(const char* defval){defaultvalue_ = defval;}
    const RString& defaultValue() const{return defaultvalue_;}
    StrTabCol(const RString& name, int32 maxchars);
    ~StrTabCol() = default;

private:
    RString value_;
    RString defaultvalue_;
    int32 maxcharnum_;
};

RATEL_NAMESPACE_END

#endif
