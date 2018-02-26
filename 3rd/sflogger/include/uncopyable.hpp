#ifndef __UNCOPYABLE_H__
#define __UNCOPYABLE_H__

#include "sfloggerpublic.h"

SFLOGGER_NAMESPACE_BEGIN
class Uncopyable
{
protected:
    Uncopyable() {}
    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&);
    const Uncopyable& operator=(const Uncopyable&);
};
NAMESPACE_END
#endif