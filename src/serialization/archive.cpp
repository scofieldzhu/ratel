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
	fileoperator_.readData((char*)(&value), sizeof(value), nullptr);
	return *this;
}

Archive& Archive::operator>>(int16_t& value)
{
	fileoperator_.readData((char*)(&value), sizeof(value), nullptr);
	return *this;
}

Archive& Archive::operator>>(RString& str)
{
	uint32_t finishedbytes = 0;
	char* buffer = new char[str.size() + 1];
	fileoperator_.readData(buffer, str.size(), &finishedbytes);	
	buffer[finishedbytes] = '\0';
	str = buffer;
	delete[] buffer;
	return *this;
}

Archive& Archive::operator>>(double& value)
{
	fileoperator_.readData((char*)(&value), sizeof(value), nullptr);
	return *this;
}

Archive& Archive::operator>>(char& value)
{
	fileoperator_.readData((char*)(&value), sizeof(value), nullptr);
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

Archive& Archive::operator<<(const RString& str)
{
	fileoperator_.writeData(str.cstr(), str.size(), nullptr);
	return *this;
}

Archive& Archive::operator<<(double value)
{
	fileoperator_.writeData((const char*)(&value), sizeof(value), nullptr);
	return *this;
}

Archive& Archive::operator<<(char value)
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
	static uint32_t stNewClsSeedIdOfWrite = 0;
	logverifymsg(serializationlogger, meta.schemano != kNewClsTag, "cannot writeClsMeta for class(%s)", meta.clsname);
	auto it = metamapofwrite_.find(&meta);
	if(it != metamapofwrite_.end()){
		uint16_t clsno = kClsNoBase | it->second;
		operator<<(clsno);
	}else{
		operator<<(kNewClsTag);
		meta.store(*this);
		metamapofwrite_.insert({&meta, ++stNewClsSeedIdOfWrite});
	}
}

SClsMeta* Archive::readClsMeta()
{
	static uint32_t stNewClsSeedIdOfRead = 0;
	uint16_t clstag = 0;
	operator>>(clstag);
	if(clstag == kNewClsTag){
		SClsMeta* meta = SClsMeta::LoadMeta(*this);
		if(meta)
			metamapofread_.insert({++stNewClsSeedIdOfRead, meta});
		return meta;
	}
	uint16_t clsindex = clstag & 0x7FFF;
	auto it = metamapofread_.find(clsindex);
	if(it == metamapofread_.end())
		slog_err(serializationlogger) << "unrecognized class index:" << clsindex << endl;
	return it->second;
}

SObjectSPtr Archive::readObject(const SClsMeta& metacls)
{
	SObjectSPtr newobj = metacls.createObject();
	if(newobj)
		newobj->serialize(*this);
	return newobj;
}

SObjectSPtr Archive::readNextObject()
{
	SClsMeta* newmeta = readClsMeta();
	if(newmeta)
		return readObject(*newmeta);
	return nullptr;
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