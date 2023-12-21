/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: string_proxy.h  
 *  Create Time: Tue Dec 19 2023
 *  Copyright (c) 2023-2023 scofieldzhu
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

#ifndef __string_proxy_h__
#define __string_proxy_h__

#include "ratel/basic/basic_export.h"
#include <cstdint>
#include <initializer_list>
#include <string>

RATEL_NAMESPACE_BEGIN

class RATEL_BASIC_API StringProxy final
{
public:
	const std::string& stdStr()const { return stdstr_;  }
	static StringProxy NewUID();
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