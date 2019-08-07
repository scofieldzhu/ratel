/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.database 
Module: sqlrecord.h 
CreateTime: 2019-7-30 21:04
=======================================================================*/
#ifndef __sqlrecord_h__
#define __sqlrecord_h__

#include "sqlfield.h"

RATEL_NAMESPACE_BEGIN

class RATEL_DATABASE_API SqlRecord
{
public:
    Variant value(int32_t pos)const;
    Variant value(const RString& name)const;
    void setValue(int32_t pos, const Variant& v);
    void setValue(const RString& name, const Variant& v);

    int32_t indexOf(const RString& name)const;
    RString fieldName(int32_t pos)const;
    SqlField field(const RString& name)const;
    SqlField field(int32_t pos)const;

    bool exists(const SqlField& field)const;
    void append(const SqlField& field);
    void replace(int32_t pos, const SqlField& field);
    void insert(int32_t pos, const SqlField& field);
    void remove(int32_t pos);
    bool empty()const;
    std::size_t count()const;
    SqlRecord();
    ~SqlRecord();

private:
    std::vector<SqlField> fields_;
};

RATEL_NAMESPACE_END

#endif
