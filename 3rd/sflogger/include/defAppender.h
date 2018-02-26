#ifndef __DEFAPPENDER_H__
#define __DEFAPPENDER_H__

#include "appender.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API DefAppender : public Appender
{    
    HEAP_OBJECT(DefAppender)

    void setFilter(Filter* filter) {  filter_ = filter; }
    Filter* getFilter() { return filter_; }    

    void setFormatter(Formatter* formatter) {  formatter_ = formatter; }
    Formatter* getFormatter() { return formatter_; }

    void setFlushFrequence(unsigned int frequence) { flush_frequence_ = frequence; }
    unsigned int getFlushFrequnce()const { return flush_frequence_; }

    int getFinishedLogCount()const { return finished_log_count_; }
            
protected:
    DefAppender(unsigned int flush_frequence);
    virtual ~DefAppender();
    virtual bool write(const Log& log, const std::xStrT& logged_msg) = 0;
    virtual void flush() = 0;

private:
    bool publish(const Log& log);
    Filter* filter_;
    Formatter* formatter_;
    unsigned int flush_frequence_;
    unsigned int finished_log_count_;
};
NAMESPACE_END
#endif
