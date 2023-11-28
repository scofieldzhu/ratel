/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: sqlfield.h 
CreateTime: 2019-7-30 21:05
=======================================================================*/
#ifndef __sqlfield_h__
#define __sqlfield_h__

#include "sqlbase.h"
#include "databasepublic.h"

RATEL_NAMESPACE_BEGIN

class RATEL_DATABASE_API SqlField final
{
public:
    void setName(const RString& name) { name_ = name; }
    const RString& name()const { return name_; }
    void setAutoValue(bool isauto) { autovalue_ = isauto; }
    bool isAutoValue()const { return autovalue_; }
    void setDefaultValue(Variant& v) { defaultvalue_ = v; }
    const Variant& defaultValue()const { return defaultvalue_; }
    bool ownDefautValue()const { return !defaultvalue_.isNull(); }
    SqlDataType dataType()const { return datatype_; }
    void setLength(int32_t len) { length_ = len; }
    int32_t length()const { return length_; }
    bool ownLength()const { return length_ < 0; }
    void setPrecision(int32_t p) { precision_ = p; }
    int32_t precision()const { return precision_; }
    bool ownPrecision()const { return precision_ < 0; }    
    void setValue(const Variant& v);
    const Variant& value()const { return value_; }
    bool isNull()const { return value_.isNull(); }
    bool isValid()const { return datatype_ == SqlDataType::kNull; }
    void setReadOnly(bool r) { readonly_ = r; }
    bool isReadOnly()const { return readonly_; }
    SqlField(const RString& name = "", SqlDataType t = SqlDataType::kNull);
    SqlField(const SqlField&);
    bool equalWith(const SqlField&)const;
    const SqlField& operator=(const SqlField&);
    bool operator==(const SqlField&)const;
    bool operator!=(const SqlField&)const;
    ~SqlField() = default;

private:
    RString name_;
    Variant value_;
    SqlDataType datatype_;
    Variant defaultvalue_;
    int32_t length_ = -1;
    int32_t precision_ = -1;
    bool autovalue_ = false;
    bool readonly_ = false;
};

RATEL_NAMESPACE_END

#endif
