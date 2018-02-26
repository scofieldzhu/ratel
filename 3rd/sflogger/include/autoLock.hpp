#ifndef __AUTO_LOCK_HPP__
#define __AUTO_LOCK_HPP__

#include "sfloggerpublic.h"

SFLOGGER_NAMESPACE_BEGIN
template <class L>
class AutoLock
{
public:
    AutoLock() { obj_.lock(); }
    ~AutoLock() { obj_.unlock(); }
private:
    L obj_;
};
NAMESPACE_END

#endif
