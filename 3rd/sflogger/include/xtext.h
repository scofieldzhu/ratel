#ifndef __XTEXT_H__
#define __XTEXT_H__

#include "platform.h"

SFLOGGER_NAMESPACE_BEGIN
SFLOGGER_API std::xStrT Int2Str(int value, int radix = 10);
SFLOGGER_API int Str2Int(const std::xStrT& str);
SFLOGGER_API std::wstring AnsiToUnicode(const char* src);
SFLOGGER_API std::string UnicodeToAnsi(const wchar_t* src);
SFLOGGER_API std::xStrT ConvertArgsToString(const xCharT* format, ...);
SFLOGGER_API std::xStrT SplitFilenameFromFullPath(const xCharT* abs_filepath, bool with_ext_flag = true);
NAMESPACE_END
#endif
