#ifndef __TRACER_H__
#define __TRACER_H__

#include "sfloggerpublic.h"

SFLOGGER_NAMESPACE_BEGIN

SFLOGGER_API void StartTrack(Logger* logger, xCharT* file, const xCharT* function, int lineno);
SFLOGGER_API void StopTrack(const xCharT* file, const xCharT* function, int lineno);
SFLOGGER_API void PlaceTrackSlot(const xCharT* file, const xCharT* function, int lineno);

struct SFLOGGER_API FunctionTracer
{
    FunctionTracer(Logger& logger, const xCharT* file,  const xCharT* function, int lineno, bool trace_time_wasted);
    ~FunctionTracer();
private:
    struct Impl;
    Impl* impl_;
};

NAMESPACE_END
#endif