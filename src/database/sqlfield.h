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

RATEL_NAMESPACE_BEGIN

class RATEL_DATABASE_API SqlField
{
public:
    void setAutoValue(bool autoval);
    bool autoValue()const;
    void setDefaultValue(Variant& v);

};

RATEL_NAMESPACE_END

#endif
