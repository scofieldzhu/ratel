#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "sfloggerpublic.h"

SFLOGGER_NAMESPACE_BEGIN

struct Level
{
    virtual int getValue() const = 0;
    virtual const xCharT* repr() const = 0;
    virtual Level* clone()const = 0;
    virtual ~Level() {}
};

struct DefLevel : public Level
{
    int getValue() const { return val_; }
    DefLevel(int val)
        :val_(val) {}
    virtual ~DefLevel() {}
private:
    int val_;
};

NAMESPACE_END
#endif