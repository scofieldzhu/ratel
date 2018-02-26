#ifndef __LEVEL_FILTER_H__
#define __LEVEL_FILTER_H__

#include "filter.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API LevelFilter : public Filter
{
    HEAP_OBJECT(LevelFilter)
    bool isLoggabled(const Log& log)const;
    void setLowerBoundLevel(const Level* level);
    const Level* getLowerBoundLevel()const;
    void setUpperBoundLevel(const Level* level);
    const Level* getUpperBoundLevel()const;     
    LevelFilter(const Level* lower = NULL, const Level* upper = NULL);    
private:        
    ~LevelFilter();
    Level* lower_bound_level_;
    Level* upper_bound_level_;
};
NAMESPACE_END
#endif
