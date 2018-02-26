#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <vector>
#include "heapobject.h"

SFLOGGER_NAMESPACE_BEGIN

struct SFLOGGER_API Logger : public HeapObject
{    
    HEAP_OBJECT(Logger)
    void destroy();
    void setFilter(Filter* filter) { filter_ = filter; }
    Filter* getFilter() const { return filter_; }
    const std::xStrT& getName()const { return name_; }
    void addAppender(Appender& appender);
    void removeAppender(Appender& appender);
    void publish(Log& log);
    void publish(const Level* level, const xCharT* fmt, ...);
    void publish(const Level* level, const xCharT* file, const xCharT* func, unsigned int lineno, const xCharT* fmt, ...);
    Logger(const std::xStrT& name);    

private:
    ~Logger(); 
    typedef std::vector<Appender*> AppenderListType;
    AppenderListType appenders_;
    std::xStrT name_;
    Filter* filter_;
};
NAMESPACE_END
#endif