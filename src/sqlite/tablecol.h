/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: tablecol.h 
CreateTime: 2018-7-28 10:51
=========================================================================*/
#ifndef __tablecol_h__
#define __tablecol_h__

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

#define DECL_COLCLS(Cls, DataType) \
    public:\
        virtual const char* dataTypeString()const{return #DataType;} \
        virtual TableCol* clone()const { return new Cls(*this); }

class TableCol
{
public:
    virtual const char* dataTypeString()const = 0;
    virtual TableCol* clone()const = 0;
    virtual RString description()const
    {
        RString desp = name() + " " + dataTypeString();
        if (primarykey_) {
            desp += " ";
            desp += PRIMARYKEY;
        }
        if (unique_) {
            desp += " ";
            desp += UNIQUE;
        }
        if (notnull_) {
            desp += " ";
            desp += NOTNULL;
        }
        return desp;
    }
    bool primaryKey()const { return primarykey_; }
    TableCol& setPrimaryKey(bool b) { 
        primarykey_ = b; 
        return *this;
    }
    bool notNull()const { return notnull_; }
    TableCol& setNotNull(bool b) {
        notnull_ = b; 
        return *this;
    }
    bool unique()const { return unique_; }
    TableCol& setUnique(bool b) {
        unique_ = b;
        return *this;
    }
    TableCol& setDefault(bool b) {
        default_ = b; 
        return *this;
    }
    bool default()const { return default_; }
    DataType dataType()const { return datatype_; }
    const RString& name()const { return name_; }
    TableCol(const RString& name, DataType dt)
        :name_(name),
        datatype_(dt)
    {}
    virtual ~TableCol(){}

protected:
    RString name_;
    DataType datatype_;
    bool primarykey_ = false;
    bool unique_ = false;
    bool notnull_ = false;
    bool default_ = false;
};

class IntCol : public TableCol
{
    DECL_COLCLS(IntCol, INTEGER)
public:
    void setValue(int32 val) { value_ = val; }
    int32 value()const { return value_; }
    IntCol& setDefaultValue(int32 val) { 
        defaultvalue_ = val; 
        return *this;
    }
    int32 defaultValue()const { return defaultvalue_; }
    IntCol(const RString& name)
        :TableCol(name, INTEGER){}
    ~IntCol() = default;

private:
    int32 value_ = 0;
    int32 defaultvalue_ = 0;
};

class DoubleCol : public TableCol 
{
    DECL_COLCLS(DoubleCol, REAL)
public:    
    void setValue(double val) { value_ = val; }
    double value()const { return value_; }
    DoubleCol& setDefaultValue(double val) {
        defaultvalue_ = val; 
        return *this;
    }
    double defaultValue()const { return defaultvalue_; }
    DoubleCol(const RString& name)
        :TableCol(name, REAL){}
    ~DoubleCol() = default;

private:
    double value_ = 0.0;
    double defaultvalue_ = 0.0;
};

class StrCol : public TableCol 
{
public:
    virtual const char* dataTypeString()const { return "VARCHAR"; }
    virtual TableCol* clone()const { return new StrCol(*this); }
    virtual RString description()const
    {
        RString desp = RString::FormatString("%s VARCHAR(%d)", name().cstr(), maxcharnum_);
        if (primaryKey()) {
            desp += " ";
            desp += PRIMARYKEY;
        }
        if (unique()) {
            desp += " ";
            desp += UNIQUE;
        }
        if (notNull()) {
            desp += " ";
            desp += NOTNULL;
        }
        return desp;
    }
    void setValue(const RString& val) { value_ = val; }
    const RString& value()const { return value_; }
    StrCol& setMaxCharNum(int32 maxcharnum) {
        maxcharnum_ = maxcharnum;
        return *this;

    }
    int32 maxCharNum()const { return maxcharnum_; }
    StrCol(const RString& name, int32 maxchars)
        :TableCol(name, STRING),
        maxcharnum_(maxchars){}
    ~StrCol() = default;

private:
    RString value_;
    int32 maxcharnum_;
};

RATEL_NAMESPACE_END
#endif
