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
	enum DataType
	{
		kNoneType,
		kIntType,
		kUIntType,
		kDoubleType,
		kStringType,
		kVoidPtrType
	};
	void setVoidPtr(vptr_t ptr);
	bool toVoidPtr(vptr_t& ptr)const;
	vptr_t toVoidPtr()const;
	void setStr(const RString& str);
	bool toStr(RString& result)const;
	RString toStr()const;
	void setInt32(int32_t val);
	bool toInt32(int32_t& result)const;
	int32_t toInt32()const;	
	void setUInt32(uint32_t val);
	bool toUInt32(uint32_t& result)const;
	uint32_t toUInt32()const;
	void setDouble(double val);
	bool toDouble(double& result)const;
	double toDouble()const;	
	bool operator==(const Variant& rhs)const;
	bool operator!=(const Variant& rhs)const;
	bool isNull()const { return isnull_; }
	void setNull();
	void setDataType(DataType t);
	DataType dataType()const { return type_; }
	Variant();
	Variant(DataType dt);
	Variant(int32_t val);
	Variant(uint32_t val);
    Variant(double val);
	Variant(const char* str);
	Variant(vptr_t ptr);
	Variant(const Variant& rhs);
	Variant& operator=(const Variant& rhs);	
    ~Variant();

private:    
    RString strvalue_;
	DataType type_;
	bool isnull_;
};

RATEL_NAMESPACE_END

#endif
