#ifndef __FILTER_H__
#define __FILTER_H__

#include "heapobject.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API Filter : public HeapObject
{
    virtual bool isLoggabled(const Log& log)const = 0;
protected:
    virtual ~Filter() {}
};
NAMESPACE_END
#endif
