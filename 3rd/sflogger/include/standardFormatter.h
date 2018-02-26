#ifndef __STANDARD_FORMATTER_H__
#define __STANDARD_FORMATTER_H__

#include "formatter.h"

SFLOGGER_NAMESPACE_BEGIN 
struct SFLOGGER_API StandardFormatter : public Formatter
{
    HEAP_OBJECT(StandardFormatter)
    std::xStrT format(const Log& log);
    StandardFormatter();    
private:
    ~StandardFormatter();
    std::xStrT handleLineBreak(const Log& log, std::xStrT& formatted_str);
};
NAMESPACE_END
#endif
