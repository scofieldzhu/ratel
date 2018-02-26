#ifndef __heapobject_h__
#define __heapobject_h__

#include "sfloggerpublic.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API HeapObject
{
    virtual void destroy() { delete this; } //only created object by new method
    virtual const xCharT* getClassName()const = 0;
protected:
    virtual ~HeapObject() {}
};

#define HEAP_OBJECT(ClassName) \
public: \
    const xCharT* getClassName()const { return _X(#ClassName);}

NAMESPACE_END
#endif