#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "sfloggerpublic.h"

SFLOGGER_NAMESPACE_BEGIN
SFLOGGER_API void DebugBreak();
SFLOGGER_API int GetCurrentTickCount();
SFLOGGER_API int GetPresentThreadId();
SFLOGGER_API void Utf8ToWideChar(const char* utf8_chars, wchar_t*& dst_chars, int* dst_buffer_size = NULL);
SFLOGGER_API void WideCharToUtf8(const wchar_t* wide_chars, char*& dst_chars, int* dst_buffer_size = NULL);
class SFLOGGER_API CriticalSectionLock
{
public:
    void lock();
    void unlock();
    CriticalSectionLock();
    ~CriticalSectionLock();
private:
    struct Impl;
    Impl* impl_;
};
NAMESPACE_END

#endif