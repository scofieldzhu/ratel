#ifndef __CONSOLE_APPENDER_H__
#define __CONSOLE_APPENDER_H__

#include "ostreamAppender.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API ConsoleAppender : public OstreamAppender
{
    HEAP_OBJECT(ConsoleAppender)
    ConsoleAppender(unsigned int flush_frequence = 1);
private:
    ~ConsoleAppender();
};
NAMESPACE_END
#endif
