#ifndef __utilmacro_h__
#define __utilmacro_h__

#include "levels.h"
#include "logger.h"
#include "tracer.h"
#include "platform.h"
#include "loggerMgr.h"
#include "logstream.h"

#define log_debug(plogger, fmt, ...) plogger->publish(SFLOGGER::debuglevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define log_info(plogger, fmt, ...) plogger->publish(SFLOGGER::infolevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define log_warn(plogger, fmt, ...) plogger->publish(SFLOGGER::warnlevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define log_err(plogger, fmt, ...) plogger->publish(SFLOGGER::errlevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define log_fatal(plogger, fmt, ...) plogger->publish(SFLOGGER::fatallevel(), __XFILE__, __XFUNCTION__, __LINE__, fmt, __VA_ARGS__)

#define slog(logger, level) SFLOGGER::LogStream(logger, level, __XFUNCTION__, __XFILE__, __LINE__)
#define slog_debug(logger) slog(logger, debuglevel())
#define slog_info(logger) slog(logger, infolevel())
#define slog_warn(logger) slog(logger, warnlevel())
#define slog_err(logger) slog(logger, errlevel())
#define slog_fatal(logger) slog(logger, fatallevel())

#define track_now(logger) SFLOGGER::StartTrack(logger, __XFILE__, __XFUNCTION__, __LINE__)
#define stop_track() SFLOGGER::StopTrack(__XFILE__, __XFUNCTION__, __LINE__)
#define mark_track_slot() SFLOGGER::PlaceTrackSlot(__XFILE__, __XFUNCTION__, __LINE__)
#define track_this_func(logger) SFLOGGER::FunctionTracer __ftracer__(logger, __XFILE__, __XFUNCTION__, __LINE__, true)

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
