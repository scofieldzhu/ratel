#ifndef __LOGGER_MGR_H__
#define __LOGGER_MGR_H__

#include <vector>
#include "sfloggerpublic.h"
#include "uncopyable.hpp"

SFLOGGER_NAMESPACE_BEGIN
class SFLOGGER_API LoggerMgr : private Uncopyable
{
public:
    static LoggerMgr* GetInst();
    void addLogger(Logger& logger);
    Logger* getLogger(const xCharT* name);   
    void reset();

private:
    LoggerMgr();    
    ~LoggerMgr();
    typedef std::vector<Logger*> LoggerListType;
    LoggerListType loggers_;
    static LoggerMgr* inst_;
};
NAMESPACE_END
#endif
