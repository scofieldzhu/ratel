/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: variant.h 
CreateTime: 2019-1-18 18:38
=========================================================================*/
#ifndef __variant_h__
#define __variant_h__

#include "rstring.h"

RATEL_NAMESPACE_BEGIN

class RATEL_BASIC_API Variant
{
public:
	void setVoidPtr(vptr_t ptr);
	bool convertToVoidPtr(vptr_t& ptr)const;
	vptr_t convertToVoidPtr()const;
	void setStr(const RString& str);
	bool convertToStr(RString& result)const;
	RString convertToStr()const;
	void setInt32(int32_t val);
	bool convertToInt32(int32_t& result)const;
	int32_t convertToInt32()const;	
	bool operator==(const Variant& rhs)const;
	bool operator!=(const Variant& rhs)const;
	Variant(int32_t val);
    Variant(double val);
	Variant(const RString& str);
	Variant(vptr_t ptr);
	Variant(const Variant& rhs);
	Variant& operator=(const Variant& rhs);	
    ~Variant();

private:    
    RString strvalue_;
	BasicType type_;
};

RATEL_NAMESPACE_END

#endif
