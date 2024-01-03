/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: string_proxy.cpp 
 *  Copyright (c) 2023-2024 scofieldzhu
 *  
 *  MIT License
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "string_proxy.h"
#include <cctype>
#include <sstream>
#include <codecvt>
#include "spdlog/spdlog.h"
#ifdef PLATFORM_WIN
#include "win_header.h"
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif

#ifdef PLATFORM_WIN
namespace
{
	template <size_t CP>
	std::wstring MultiBytesToUnicode(const std::string& multi_bytes)
	{
		if(multi_bytes.empty())
			return std::wstring();
		int capacity = ::MultiByteToWideChar(CP, 0, multi_bytes.data(), (int)multi_bytes.size(), NULL, 0);
		std::wstring result_wstr(capacity + 1, 0);
		::MultiByteToWideChar(CP, 0, multi_bytes.data(), (int)multi_bytes.size(), result_wstr.data(), capacity);
		return result_wstr;
	}

	template <size_t CP>
	std::string UnicodeToMultiBytes(const std::wstring& unicode_str)
	{
		if(unicode_str.empty())
			return std::string();
		int capacity = ::WideCharToMultiByte(CP, 0, unicode_str.c_str(), (int)unicode_str.size(), NULL, 0, NULL, NULL);
		std::string result_str(capacity + 1, 0);
		::WideCharToMultiByte(CP, 0, unicode_str.c_str(), (int)unicode_str.size(), result_str.data(), capacity, NULL, NULL);
		return result_str;
	}
}

#endif

RATEL_NAMESPACE_BEGIN

StringProxy::StringProxy()
{
}

StringProxy::StringProxy(size_t count, char ch)
	:stdstr_(count, ch)
{
}

StringProxy::StringProxy(const StringProxy& other, size_t pos)
	:stdstr_(other.stdstr_, pos)
{
}

StringProxy::StringProxy(const StringProxy& other, size_t pos, size_t count)
	:stdstr_(other.stdstr_, pos, count)
{
}

StringProxy::StringProxy(const char* s, size_t count)
	:stdstr_(s, count)
{
}

StringProxy::StringProxy(const char* s)
	:stdstr_(s)
{
}

StringProxy::StringProxy(const StringProxy& other)
	:stdstr_(other.stdstr_)
{
}

StringProxy::StringProxy(StringProxy&& other) noexcept
	:stdstr_(std::move(other.stdstr_))
{
}

StringProxy::StringProxy(std::initializer_list<char> ilist)
	:stdstr_(ilist)
{
}

StringProxy::~StringProxy()
{
}

StringProxy& StringProxy::operator=(std::initializer_list<char> ilist)
{
	stdstr_ = ilist;
	return *this;
}

StringProxy& StringProxy::operator=(char ch)
{
	stdstr_ = ch;
	return *this;
}

StringProxy& StringProxy::operator=(const char* s)
{
	stdstr_ = s;
	return *this;
}

StringProxy& StringProxy::operator=(StringProxy&& other)
{
	stdstr_ = std::move(other.stdstr_);
	return *this;
}

StringProxy& StringProxy::operator=(const StringProxy& other)
{
	stdstr_ = other.stdstr_;
	return *this;
}

StringProxy& StringProxy::operator=(std::string&& str)
{
	stdstr_ = std::move(str);
	return *this;
}

StringProxy& StringProxy::operator=(const std::string& str)
{
	stdstr_ = str;
	return *this;
}

StringProxy StringProxy::NewUID()
{
	char buffer[50] = {'\0'};
#ifdef PLATFORM_WIN
	GUID guid;
	::CoCreateGuid(&guid);
	std::sprintf(buffer,
		"%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
	);
#else
	uuid_t uuid;
	uuid_generate_random(uuid);
	uuid_unparse(uuid, buffer);
#endif	
	return StringProxy(buffer);
}

void StringProxy::toLower()
{
	for(auto& ch : stdstr_){
		if(std::isalpha(ch)){
			ch = std::tolower(ch);
		}
	}
}

void StringProxy::toUpper()
{
	for(auto& ch : stdstr_){
		if(std::isalpha(ch)){
			ch = std::toupper(ch);
		}
	}
}

namespace{
	template <typename RT> 
	RT ConvertStrToNumber(const std::string& str)
	{
		std::stringstream ss;
		ss << str;
		RT val = 0;
		ss >> val;
		return val;
	}

	template <typename NT>
	std::string ConvertNumberToString(NT num)
	{
		return std::to_string(num);
	}
}

int32_t StringProxy::toInt() const
{
	return ConvertStrToNumber<int32_t>(stdstr_);
}

uint32_t StringProxy::toUInt() const
{
	return ConvertStrToNumber<uint32_t>(stdstr_);
}

int64_t StringProxy::toLargeInt() const
{
	return ConvertStrToNumber<int64_t>(stdstr_);
}

uint64_t StringProxy::toLargeUInt() const
{
	return ConvertStrToNumber<uint64_t>(stdstr_);
}

double StringProxy::toDouble() const
{
	return ConvertStrToNumber<double>(stdstr_);
}

std::string StringProxy::decodeToLocale() const
{
#ifdef PLATFORM_WIN
	std::wstring ustr = MultiBytesToUnicode<CP_UTF8>(stdstr_);
	return UnicodeToMultiBytes<CP_ACP>(ustr);
#else
	spdlog::error("Method not implmented in non-win platform!");
	return "";
#endif
}

std::wstring StringProxy::decodeToWString() const
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
	return cvt.from_bytes(stdstr_);
}

StringProxy StringProxy::FromLocale(const char* source)
{
	StringProxy p;
	p.encodeFromLocale(source);
	return p;
}

void StringProxy::encodeFromLocale(const char* source)
{
#ifdef PLATFORM_WIN
	std::wstring ustr = MultiBytesToUnicode<CP_ACP>(source);
	if(!ustr.empty())
		stdstr_ = UnicodeToMultiBytes<CP_UTF8>(ustr);
#else
	SPDLOG_ERROR("Method not implmented in non-win platform!");
#endif
}

StringProxy StringProxy::FromWString(const std::wstring& source)
{
	StringProxy p;
	p.encodeFromWString(source);
	return p;
}

void StringProxy::encodeFromWString(const std::wstring& source)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
	stdstr_ = cvt.to_bytes(source);
}

StringProxy StringProxy::FromInt(int32_t val)
{
	return StringProxy(ConvertNumberToString((int)val));
}

StringProxy StringProxy::FromUInt(uint32_t val)
{
	return StringProxy(ConvertNumberToString((unsigned long)val));
}

StringProxy StringProxy::FromLargeInt(int64_t val)
{
	return StringProxy(ConvertNumberToString((long long)val));
}

StringProxy StringProxy::FromLargeUInt(uint64_t val)
{
	return StringProxy(ConvertNumberToString((unsigned long long)val));
}

StringProxy StringProxy::FromDouble(double val)
{
	return StringProxy(ConvertNumberToString(val));
}

ByteVec StringProxy::serializeToBytes()const
{
	const auto total_byte_size = stdstr_.size() + kUIntSize;
	ByteVec bv(total_byte_size, 0);
	unsigned int required_size = (unsigned int)stdstr_.size();
	memcpy(bv.data(), &required_size, kUIntSize);	
	if(!stdstr_.empty())
		memcpy(bv.data() + kUIntSize, stdstr_.data(), required_size);
	return bv;
}

size_t StringProxy::loadBytes(ConsBytePtr buffer, size_t size)
{
	if(buffer == nullptr || size < kUIntSize)
		return 0;
	unsigned int required_size = *(const unsigned int*)buffer;
	if(required_size == 0){
		stdstr_.clear();
	}else{
		if(size < required_size + kUIntSize) //data lost!
			return 0;
		stdstr_.resize(required_size);
		memcpy((void*)stdstr_.data(), buffer + kUIntSize, required_size);
	}
	return stdstr_.size() + kUIntSize;
}

RATEL_NAMESPACE_END
