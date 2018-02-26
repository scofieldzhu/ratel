#ifndef __OSTREAM_APPENDER_H__
#define __OSTREAM_APPENDER_H__

#include <ostream>
#include "defAppender.h"

SFLOGGER_NAMESPACE_BEGIN

struct SFLOGGER_API OstreamAppender : public DefAppender
{        
    HEAP_OBJECT(OstreamAppender)    
protected:
    virtual ~OstreamAppender();
    typedef std::basic_ostream<xCharT> OstreamT;
    OstreamAppender(OstreamT& os, unsigned int flush_frequence);
    bool write(const Log& log, const std::xStrT& logged_msg);
    void flush();
    OstreamT* os_;
};

NAMESPACE_END
#endif
