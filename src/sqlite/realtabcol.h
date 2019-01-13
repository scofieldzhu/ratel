/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: realtabcol.h 
CreateTime: 2019-1-13 20:43
=========================================================================*/
#ifndef __realtabcol_h__
#define __realtabcol_h__

#include "dbtablecol.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SQLITE_API RealTabCol : public DbTableCol
{
    DECL_COLCLS(RealTabCol, REAL)
public:
    virtual RString description()const;
    void setValue(double val){value_ = val;}
    double value()const{return value_;}
    RealTabCol& setDefaultValue(double val);
    double defaultValue()const{return defaultvalue_;}
    RealTabCol(const RString& name);
    ~RealTabCol() = default;

private:
    double value_ = 0.0;
    double defaultvalue_ = 0.0;
};

RATEL_NAMESPACE_END

#endif

