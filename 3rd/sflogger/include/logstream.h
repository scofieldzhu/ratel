#ifndef __logstream_h__
#define __logstream_h__

#include <sstream>
#include <type_traits>
#include "autoLock.hpp"
#include "platform.h"
#include "log.h"

SFLOGGER_NAMESPACE_BEGIN

class SFLOGGER_API LogStream
{
    using osstream_type = std::basic_ostringstream<xCharT>;
public:    
    template <class T>
    LogStream& operator<<(T& obj){
        if(std::is_object<T>::value) 
            oss_ << obj.repr();        
        return *this;
    }
    LogStream& operator<<(bool val);
    LogStream& operator<<(short val);
    LogStream& operator<<(unsigned short val);
    LogStream& operator<<(int val);
    LogStream& operator<<(unsigned int val);
    LogStream& operator<<(long val);
    LogStream& operator<<(unsigned long val);
    LogStream& operator<<(long long val);
    LogStream& operator<<(unsigned long long val);
    LogStream& operator<<(float val);
    LogStream& operator<<(double val);
    LogStream& operator<<(long double val);
    LogStream& operator<<(void* val);
    LogStream& operator<<(const xCharT* val);
    LogStream& enableAutoSpace();
    LogStream& disableAutoSpace();
    bool autoSpace()const { return autospace_; }
    using ostream_type = std::basic_ostream<xCharT, std::char_traits<xCharT>>;
    LogStream& operator<<(ostream_type& (*func)(ostream_type&));
    LogStream(Logger* logger, const Level* level, const xCharT* func, const xCharT* absfilename, const int lineno);
    LogStream(Logger* logger, Log& thislog);
    ~LogStream();

private:
    AutoLock<CriticalSectionLock> autolock_;
    Logger* logger_;
    Log thislog_;
    osstream_type oss_;
    bool autospace_ = false;    
};

NAMESPACE_END
#endif
