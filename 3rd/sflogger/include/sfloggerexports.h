#ifndef __DR2_EXPORTER_H__
#define __DR2_EXPORTER_H__

#ifdef SFLOGGER_EXPORTS
    #define SFLOGGER_API __declspec(dllexport)
#else
    #define SFLOGGER_API __declspec(dllimport)
#endif

#endif