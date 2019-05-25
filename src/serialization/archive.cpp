/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: archive.cpp 
CreateTime: 2019-4-18 21:24
=========================================================================*/
#include "archive.h"
#include "sobject.h"
#include "serializationlogger.h"

RATEL_NAMESPACE_BEGIN

namespace{
	const uint16_t kClsNoBase = 0x8000;
	const uint16_t kNewClsTag = 0xFFFF;
	uint16_t st_NextClsSeedId = 1;
}
Archive::Archive(std::wstring& fn, bool isstore)
	:fileoperator_(fn),
	isstored_(isstore)
{}

Archive::~Archive()
{}

Archive& Archive::operator>>(uint32_t& value)
{
	fileoperator_.readData((char*)(&value), sizeof(value), nullptr);
	return *this;
}

Archive& Archive::operator>>(int32_t& value)
{
	fileoperator_.readData((char*)(&value), sizeof(value), nullptr);
	return *this;
}

Archive& Archive::operator>>(uint16_t& value)
{
	fileoperator_.readData((char*)(&value), sizeof(uint16_t), nullptr);
	return *this;
}

Archive& Archive::operator>>(int16_t& value)
{
	fileoperator_.readData((char*)(&value), sizeof(uint16_t), nullptr);
	return *this;
}

Archive& Archive::operator<<(uint32_t value)
{
	fileoperator_.writeData((const char*)(&value), sizeof(value), nullptr);
	return *this;
}

Archive& Archive::operator<<(int32_t value)
{
	fileoperator_.writeData((const char*)(&value), sizeof(value), nullptr);
	return *this;
}

Archive& Archive::operator<<(uint16_t value)
{
	fileoperator_.writeData((const char*)(&value), sizeof(value), nullptr);
	return *this;
}

Archive& Archive::operator<<(int16_t value)
{
	fileoperator_.writeData((const char*)(&value), sizeof(value), nullptr);
	return *this;
}

bool Archive::readData(char* data, uint32_t size, uint32_t* actualsize)
{
	return fileoperator_.readData(data, size, actualsize);
}

void Archive::writeData(const char* data, uint32_t size)
{
	fileoperator_.writeData(data, size);
}

void Archive::writeClsMeta(const SClsMeta& meta)
{
	logverifymsg(serializationlogger, meta.schemano != kNewClsTag, "cannot writeClsMeta for class(%s)", meta.clsname);
	auto it = clsmetamap_.find(&meta);
	if(it != clsmetamap_.end()){
		uint16_t clsindex = it->second;
		operator<<(kClsNoBase | clsindex);
	}else{
		operator<<(kNewClsTag);
		meta.store(*this);
		clsmetamap_.insert({&meta, st_NextClsSeedId++});
	}
}

SObject* Archive::readObject(SClsMeta* metacls)
{
	SObject* newobj = metacls ? metacls->createObject() : nullptr;
	if(newobj)
		newobj->serialize(*this);
	return newobj;
}

void Archive::writeObject(const SObject& obj)
{
	SClsMeta& meta = obj.getClsMeta();
	writeClsMeta(meta);		
	const_cast<SObject&>(obj).serialize(*this);
}

Archive& operator<<(Archive& ar, const SObject& obj)
{
	ar.writeObject(obj);
	return ar;
}

RATEL_NAMESPACE_END