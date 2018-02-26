#ifndef __LOG_H__
#define __LOG_H__

#include "sfloggerpublic.h"
#include "xtime.h"

SFLOGGER_NAMESPACE_BEGIN

struct SFLOGGER_API Log
{    
    Log& operator=(const Log& rhs);
    Log(const Log& rhs);
    Log(const Level* level);

    const Level* level_;
    std::xStrT logger_name_;
    std::xStrT content_;
    std::xStrT func_name_;
    std::xStrT filename_;
    unsigned int lineno_;
    unsigned int thread_id_;
    DateTime timestamp_;    
};

NAMESPACE_END
#endif