/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: dbtablecol.h 
CreateTime: 2018-7-28 10:51
=========================================================================*/
#ifndef __dbtablecol_h__
#define __dbtablecol_h__

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

#define DECL_COLCLS(Cls, DataType) \
    public:\
        virtual const char* dataTypeString()const{return #DataType;} \
        virtual DbTableCol* clone()const { return new Cls(*this); }

class RATEL_SQLITE_API DbTableCol
{
public:
    virtual const char* dataTypeString()const = 0;
    virtual DbTableCol* clone()const = 0;
    virtual RString description()const;
    bool primaryKey() const{ return primarykey_;}
    DbTableCol& setPrimaryKey(bool b);
    DbTableCol& primaryKeyOn();
    bool isNotNull() const{return notnull_;}
    DbTableCol& setNotNull(bool b);
    DbTableCol& notNullOn();
    bool unique() const{return unique_;}
    DbTableCol& setUnique(bool b);
    DbTableCol& uniqueOn();
    DbTableCol& setDefault(bool b);
    bool isDefault() const{return default_;}
    DbTableCol& defaultOn();
    DataType dataType() const{return datatype_;}
    const RString& name() const{return name_;}
    DbTableCol(const RString& name, DataType dt);
    virtual ~DbTableCol();

protected:
    RString name_;
    DataType datatype_;
    bool primarykey_ = false;
    bool unique_ = false;
    bool notnull_ = false;
    bool default_ = false;
};

RATEL_NAMESPACE_END

#endif
