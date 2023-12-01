#ifndef __string_proxy_h__
#define __string_proxy_h__

#include "basic_export.h"
#include <cstdint>
#include <initializer_list>
#include <string>

RATEL_NAMESPACE_BEGIN

class RATEL_BASIC_API StringProxy final
{
public:
	const std::string& stdStr()const { return stdstr_;  }
	void toLower();
	void toUpper();
	static StringProxy FromInt(int32_t ival);
	static StringProxy FromUInt(uint32_t ival);
	static StringProxy FromLargeInt(int64_t ival);
	static StringProxy FromLargeUInt(uint64_t ival);
	static StringProxy FromDouble(double ival);
	int32_t toInt()const;
	uint32_t toUInt()const;
	int64_t toLargeInt()const;
	uint64_t toLargeUInt()const;
	double toDouble()const;
	std::string decodeToLocale()const;
	std::wstring decodeToWString()const;
	static StringProxy FromLocale(const char* source);
	void encodeFromLocale(const char* source);
	static StringProxy FromWString(const std::wstring& source);
	void encodeFromWString(const std::wstring& source);
	StringProxy& operator=(const std::string& str);
	StringProxy& operator=(std::string&& str);
	StringProxy& operator=(const StringProxy& other);
	StringProxy& operator=(StringProxy&& other);
	StringProxy& operator=(const char* s);
	StringProxy& operator=(char ch);
	StringProxy& operator=(std::initializer_list<char> ilist);
	StringProxy();
	StringProxy(size_t count, char ch);
	StringProxy(const StringProxy& other, size_t pos);
	StringProxy(const StringProxy& other, size_t pos, size_t count);
	StringProxy(const char* s, size_t count);
	StringProxy(const char* s);
	template< class InputIt >
	StringProxy(InputIt first, InputIt last)
		:stdstr_(first, last)
	{}
	StringProxy(const StringProxy& other);
	StringProxy(StringProxy&& other) noexcept;
	StringProxy(std::initializer_list<char> ilist);
	template< class StringViewLike >
	explicit StringProxy(const StringViewLike& t)
		:stdstr_(t)
	{}
	template< class StringViewLike >
	StringProxy(const StringViewLike& t, size_t pos, size_t n)
		:stdstr_(t, pos, n)
	{}
	StringProxy(std::nullptr_t) = delete;
	~StringProxy();

private:
	std::string stdstr_; //utf8-encode
};

RATEL_NAMESPACE_END

#endif