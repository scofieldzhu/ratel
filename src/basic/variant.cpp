/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: variant.cpp 
CreateTime: 2019-1-26 12:16
=========================================================================*/
#include "variant.h"
#include "basiclogger.h"

#define LOG_MISMATCHED_TYPE_DETECTED() slog_err(basiclogger) << "mismatched type detected!" << endl;
#define CHECK_MISMATCHED_TYPE(t) if(type_ != t){ LOG_MISMATCHED_TYPE_DETECTED(); return false; }
#define VERIFY_MATCHED_TYPE(t) logverifymsg(basiclogger, t != type_, "mismatched type detected!");

RATEL_NAMESPACE_BEGIN
Variant::Variant(int32_t val)
	:strvalue_(RString::FromInt32(val)),
	type_(kIntType)
{}

Variant::Variant(double val)
	:strvalue_(RString::FromDouble(val)),
	type_(kDoubleType)
{}

Variant::Variant(vptr_t ptr)
	:strvalue_(RString::FromInt64(int64_t(ptr))),
	type_(kVoidPtrType)
{}

Variant::Variant(const RString& str)
	:strvalue_(str),
	type_(kStringType)
{}

Variant::Variant(const Variant& rhs)
	:strvalue_(rhs.strvalue_),
	type_(rhs.type_)
{}

Variant::~Variant()
{}

bool Variant::operator!=(const Variant& rhs) const
{
	return !operator==(rhs);	
}

bool Variant::operator==(const Variant& rhs) const
{
	return strvalue_ == rhs.strvalue_ && type_ == rhs.type_;
}

Variant& Variant::operator=(const Variant& rhs)
{
	strvalue_ = rhs.strvalue_;
	type_ = rhs.type_;
	return *this;
}

void Variant::setVoidPtr(vptr_t ptr)
{
	strvalue_.FromInt64(int64_t(ptr));
	type_ = kVoidPtrType;
}

vptr_t Variant::convertToVoidPtr() const
{
	VERIFY_MATCHED_TYPE(kVoidPtrType)
	return reinterpret_cast<vptr_t>(strvalue_.toInt64());	
}

bool Variant::convertToVoidPtr(vptr_t& result) const
{
	CHECK_MISMATCHED_TYPE(kVoidPtrType);
	result = convertToVoidPtr();
	return true;
}

void Variant::setStr(const RString& str)
{
	strvalue_ = str;
	type_ = kStringType;
}

RString Variant::convertToStr() const
{
	VERIFY_MATCHED_TYPE(kStringType)
	return strvalue_;
}

bool Variant::convertToStr(RString& result) const
{
	CHECK_MISMATCHED_TYPE(kStringType);
	result = convertToStr();
	return true;
}

void Variant::setInt32(int32_t val)
{
	strvalue_.fromInt32(val);
	type_ = kIntType;
}

int32_t Variant::convertToInt32() const
{
	VERIFY_MATCHED_TYPE(kIntType)
	return strvalue_.toInt32();
}

bool Variant::convertToInt32(int32_t& result) const
{
	CHECK_MISMATCHED_TYPE(kIntType)
	result = convertToInt32();
	return true;
}

RATEL_NAMESPACE_END
