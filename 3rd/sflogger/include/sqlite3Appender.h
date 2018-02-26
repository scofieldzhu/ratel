#ifndef __SQLITE3_APPENDER_H__
#define __SQLITE3_APPENDER_H__

#include "defAppender.h"

SFLOGGER_NAMESPACE_BEGIN
struct SFLOGGER_API Sqlite3Appender : public DefAppender
{
    HEAP_OBJECT(Sqlite3Appender)
    Sqlite3Appender(const std::xStrT& db_file, unsigned int flush_frequence);    
private:    
    ~Sqlite3Appender();
    bool write(const Log& log, const std::xStrT& logged_msg);
    virtual void flush();
    struct Impl;
    Impl* impl_;
};
SFLOGGER_API const std::string& GetSqlite3LastError();
NAMESPACE_END

#endif
