/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: variant.cpp 
CreateTime: 2019-1-26 12:16
=========================================================================*/
//#include "variant.h"
//#include "basiclogger.h"
//
//#define LOG_MISMATCHED_TYPE_DETECTED() slog_err(basiclogger) << "mismatched type detected!" << endl;
//#define CHECK_MISMATCHED_TYPE(t) if(type_ != t){ LOG_MISMATCHED_TYPE_DETECTED(); return false; }
//#define VERIFY_MATCHED_TYPE(t) logverifymsg(basiclogger, t == type_, "mismatched type detected!");
//
//RATEL_NAMESPACE_BEGIN
//Variant::Variant()
//	:type_(kNoneType),
//	isnull_(true)
//{}
//
//Variant::Variant(DataType dt)
//	:type_(dt),
//	isnull_(true)
//{}
//
//Variant::Variant(int32_t val)
//	:strvalue_(RString::FromInt32(val)),
//	type_(kIntType),
//	isnull_(false)
//{}
//
//Variant::Variant(uint32_t val)
//	:strvalue_(RString::FromUInt32(val)),
//	type_(kUIntType),
//	isnull_(false)
//{}
//
//Variant::Variant(double val)
//	:strvalue_(RString::FromDouble(val)),
//	type_(kDoubleType),
//	isnull_(false)
//{}
//
//Variant::Variant(vptr_t ptr)
//	:strvalue_(RString::FromInt64(int64_t(ptr))),
//	type_(kVoidPtrType),
//	isnull_(false)
//{}
//
//Variant::Variant(const char* str)
//	:strvalue_(str),
//	type_(kStringType),
//	isnull_(false)
//{}
//
//Variant::Variant(const Variant& rhs)
//	:strvalue_(rhs.strvalue_),
//	type_(rhs.type_),
//	isnull_(rhs.isnull_)
//{}
//
//Variant::~Variant()
//{}
//
//bool Variant::operator!=(const Variant& rhs) const
//{
//	return !operator==(rhs);	
//}
//
//bool Variant::operator==(const Variant& rhs) const
//{
//	return strvalue_ == rhs.strvalue_ && type_ == rhs.type_ && isnull_ == rhs.isnull_;
//}
//
//Variant& Variant::operator=(const Variant& rhs)
//{
//	strvalue_ = rhs.strvalue_;
//	type_ = rhs.type_;
//	isnull_ = rhs.isnull_;
//	return *this;
//}
//
//void Variant::setVoidPtr(vptr_t ptr)
//{
//	strvalue_.FromInt64(int64_t(ptr));
//	type_ = kVoidPtrType;
//	isnull_ = false;
//}
//
//vptr_t Variant::toVoidPtr() const
//{
//	VERIFY_MATCHED_TYPE(kVoidPtrType)
//	return reinterpret_cast<vptr_t>(strvalue_.toInt64());	
//}
//
//bool Variant::toVoidPtr(vptr_t& result) const
//{
//	CHECK_MISMATCHED_TYPE(kVoidPtrType);
//	result = toVoidPtr();
//	return true;
//}
//
//void Variant::setStr(const RString& str)
//{
//	strvalue_ = str;
//	type_ = kStringType;
//	isnull_ = false;
//}
//
//RString Variant::toStr() const
//{
//	VERIFY_MATCHED_TYPE(kStringType)
//	return strvalue_;
//}
//
//bool Variant::toStr(RString& result) const
//{
//	CHECK_MISMATCHED_TYPE(kStringType);
//	result = toStr();
//	return true;
//}
//
//void Variant::setInt32(int32_t val)
//{
//	strvalue_.fromInt32(val);
//	type_ = kIntType;
//	isnull_ = false;
//}
//
//int32_t Variant::toInt32() const
//{
//	VERIFY_MATCHED_TYPE(kIntType)
//	return strvalue_.toInt32();
//}
//
//bool Variant::toInt32(int32_t& result) const
//{
//	CHECK_MISMATCHED_TYPE(kIntType)
//	result = toInt32();
//	return true;
//}
//
//void Variant::setUInt32(uint32_t val)
//{
//	strvalue_.fromUInt32(val);
//	type_ = kUIntType;
//	isnull_ = false;
//}
//
//bool Variant::toUInt32(uint32_t& result) const
//{
//	CHECK_MISMATCHED_TYPE(kUIntType)
//	result = toUInt32();
//	return true;
//}
//
//uint32_t Variant::toUInt32() const
//{
//	VERIFY_MATCHED_TYPE(kUIntType)
//	return strvalue_.toUInt32();
//}
//
//void Variant::setDouble(double val)
//{
//	strvalue_.fromDouble(val);
//	type_ = kDoubleType;
//	isnull_ = false;
//}
//
//bool Variant::toDouble(double& result) const
//{
//	CHECK_MISMATCHED_TYPE(kDoubleType)
//	result = toDouble();
//	return true;
//}
//
//double Variant::toDouble() const
//{
//	VERIFY_MATCHED_TYPE(kDoubleType)
//	return strvalue_.toDouble();
//}
//
//void Variant::setNull()
//{
//	isnull_ = true;
//}
//
//void Variant::setDataType(DataType t)
//{
//	type_ = t;
//	isnull_ = true;
//}
//
//RATEL_NAMESPACE_END
//
//
