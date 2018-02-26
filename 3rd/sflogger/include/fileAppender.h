#ifndef __FILE_APPENDER_H__
#define __FILE_APPENDER_H__

#include "ostreamAppender.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API FileAppender : public OstreamAppender
{
    HEAP_OBJECT(FileAppender);
    FileAppender(const std::xStrT& filepath, unsigned int flush_frequence);
private:
    ~FileAppender();
};
NAMESPACE_END
#endif
