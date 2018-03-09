#ifndef __utilmacro_h__
#define __utilmacro_h__

#include "levels.h"
#include "logger.h"
#include "tracer.h"
#include "platform.h"
#include "loggerMgr.h"
#include "logstream.h"

#define log_debug(plogger, fmt, ...) if(plogger) plogger->publish(SFLOGGER::debuglevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define log_info(plogger, fmt, ...) if(plogger) plogger->publish(SFLOGGER::infolevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define log_warn(plogger, fmt, ...) if(plogger) plogger->publish(SFLOGGER::warnlevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define log_err(plogger, fmt, ...) if(plogger) plogger->publish(SFLOGGER::errlevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define log_fatal(plogger, fmt, ...) if(plogger) plogger->publish(SFLOGGER::fatallevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)

#define slog(plogger, level) SFLOGGER::LogStream(plogger, level, __XFUNCTION__, __XFILE__, __LINE__)
#define slog_debug(plogger) slog(plogger, debuglevel())
#define slog_info(plogger) slog(plogger, infolevel())
#define slog_warn(plogger) slog(plogger, warnlevel())
#define slog_err(plogger) slog(plogger, errlevel())
#define slog_fatal(plogger) slog(plogger, fatallevel())

#define track_now(plogger) SFLOGGER::StartTrack(plogger, __XFILE__, __XFUNCTION__, __LINE__)
#define stop_track() SFLOGGER::StopTrack(__XFILE__, __XFUNCTION__, __LINE__)
#define mark_track_slot() SFLOGGER::PlaceTrackSlot(__XFILE__, __XFUNCTION__, __LINE__)
#define track_this_func(plogger) if(plogger) SFLOGGER::FunctionTracer __ftracer__(*plogger, __XFILE__, __XFUNCTION__, __LINE__, true)

#define logverify(pLogger, cond) \
    if(!(cond)){ \
        log_fatal(pLogger, _X("Condition:%s Failed!\r\n"), #cond); \
        SFLOGGER::DebugBreak(); \
    }

#define logverifymsg(pLogger, cond, fmt, ...) \
    if(!(cond)) {\
        log_fatal(pLogger, fmt, __VA_ARGS__);  \
        SFLOGGER::DebugBreak(); \
    }

#define getlogger(name) SFLOGGER::LoggerMgr::GetInst()->getLogger(name)

#pragma warning(disable:4275)

#endif
