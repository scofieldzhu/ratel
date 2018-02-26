#ifndef __FORMATTER_H__
#define __FORMATTER_H__

#include "heapobject.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API Formatter : public HeapObject
{
    virtual std::xStrT format(const Log& log) = 0;
protected:
    virtual ~Formatter() {}
};
NAMESPACE_END
#endif