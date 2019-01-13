/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: inttabcol.h 
CreateTime: 2019-1-13 20:41
=========================================================================*/
#ifndef __inttabcol_h__
#define __inttabcol_h__

#include "dbtablecol.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API IntTabCol : public DbTableCol
{
    DECL_COLCLS(IntTabCol, INTEGER)
public:
    virtual RString description()const;
    void setValue(int32 val){ value_ = val;}
    int32 value()const{return value_;}
    IntTabCol& setDefaultValue(int32 val);
    int32 defaultValue()const{return defaultvalue_;}
    IntTabCol(const RString& name);
    ~IntTabCol() = default;

private:
    int32 value_ = 0;
    int32 defaultvalue_ = 0;
};

RATEL_NAMESPACE_END

#endif
