#ifndef __APPENDER_H__
#define __APPENDER_H__

#include "heapobject.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API Appender : public HeapObject
{
    virtual bool publish(const Log& log) = 0;    
    virtual void setFilter(Filter* filter) = 0;
    virtual Filter* getFilter() = 0;    
    virtual void setFormatter(Formatter* formatter) = 0;
    virtual Formatter* getFormatter() = 0;    
    virtual void destroy();
protected:
    virtual ~Appender() {}
};
NAMESPACE_END
#endif