#ifndef __XTIME_H__
#define __XTIME_H__

#include "sfloggerpublic.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API Date
{
    Date(int y = 1900, int m = 1, int d = 1)
        :year_(y),
        mon_(m),
        mday_(d)
    {}
    int year_;
    int mon_;
    int mday_;
};

struct SFLOGGER_API DayTime
{
    DayTime(int h = 0, int m = 0, int s = 0, int ms = 0)
        :hour_(h),
        min_(m),
        sec_(s),
        millisec_(ms)
    {}
    int hour_;
    int min_;
    int sec_;
    int millisec_;
};

struct SFLOGGER_API DateTime
{
    static DateTime Now();
    std::xStrT repr()const;
    DateTime()
    {}
    DateTime(const Date& d, const DayTime& t)
        :date_(d),
        daytime_(t)
    {}
    DateTime(int y, int m, int d, int h, int min, int s, int ms)
        :date_(y, m, d),
        daytime_(h, min, s, ms)
    {}
    ~DateTime()
    {}
    Date date_;
    DayTime daytime_;
};
NAMESPACE_END

#endif