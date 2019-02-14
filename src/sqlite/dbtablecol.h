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

#include "dbglobal.h"
#include "propdict.h"

RATEL_NAMESPACE_BEGIN

class SqlDataMeta;
class DbTableCol
{
public:    
    const RString& name() const { return name_; }
	virtual RString createSql()const;
	PropDict& propDict() { return colpropdict_; }
	const PropDict& propDict()const { return colpropdict_; }
	void setDataMeta(const SqlDataMeta* meta) { sqldatameta_ = meta; }
	const SqlDataMeta* dataMeta()const {return sqldatameta_; }
    DbTableCol(const RString& name);
    virtual ~DbTableCol() = default;

private:
    RString name_;
	PropDict colpropdict_;	
	const SqlDataMeta* sqldatameta_ = nullptr;	
};

RATEL_NAMESPACE_END

#endif
