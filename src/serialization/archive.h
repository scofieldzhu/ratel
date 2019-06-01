/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.serialization 
Module: archive.h 
CreateTime: 2019-4-18 20:55
=========================================================================*/
#ifndef __archive_h__
#define __archive_h__

#include "serializationpublic.h"
#include "agilefileoperator.h"

RATEL_NAMESPACE_BEGIN

class RATEL_SERIALIZATION_API Archive
{
public:
	Archive& operator>>(uint16_t& value);
	Archive& operator<<(uint16_t value);
	Archive& operator>>(int16_t& value);	
	Archive& operator<<(int16_t value);
	Archive& operator>>(uint32_t& value);
	Archive& operator<<(uint32_t value);
	Archive& operator<<(int32_t value);
	Archive& operator>>(int32_t& value);		
	Archive& operator<<(double value);
	Archive& operator>>(double& value);
	Archive& operator<<(const RString& str);
	Archive& operator>>(RString& str);
	bool readData(char* data, uint32_t size, uint32_t* actualsize);
	void writeData(const char* data, uint32_t size);	
	void writeClsMeta(const SClsMeta& meta);
	SClsMeta* readClsMeta();
	SObject* readObject(const SClsMeta& metacls);
	SObject* readNextObject();
	void writeObject(const SObject& obj);
	bool isStored()const { return isstored_; }
	Archive(std::wstring& fn, bool isstore);
	virtual ~Archive();

private:
	friend RATEL_SERIALIZATION_API Archive& operator<<(Archive& ar, const SObject& obj);
	AgileFileOperator fileoperator_;
	bool isstored_;
	std::map<const SClsMeta*, uint16_t> metamapofwrite_;
	std::map<uint16_t, SClsMeta*> metamapofread_;
};

RATEL_SERIALIZATION_API Archive& operator<<(Archive& ar, const SObject& obj);

RATEL_NAMESPACE_END

#endif
