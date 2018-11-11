#include <iostream>
#include <windows.h>
#include "package.h"
#include "path.h"
#include "defAppender.h"
#include "logger.h"
#include "standardFormatter.h"
#include "sqlitelogger.h"
#include "pkglogger.h"
using namespace std;
USING_RATEL
USING_SFLOGGER

class ConsoleAppender : public DefAppender
{
public:
    bool write(const Log& log, const std::xStrT& logged_msg)
    {
        ::OutputDebugStringA(logged_msg.c_str());
        return true;
    }
    void flush(){}
    ConsoleAppender()
        :DefAppender(1)
    {}
};

Logger* g_AppLogger = nullptr;

void InitLogger()
{
    g_AppLogger = new Logger("app");
    ConsoleAppender* consoleappender = new ConsoleAppender();    
    consoleappender->setFormatter(new StandardFormatter());
    g_AppLogger->addAppender(*consoleappender);
    
    RATEL::sqlite::SetLogger(g_AppLogger);
    RATEL::package::SetLogger(g_AppLogger);
}

void TestCase_TestPackage()
{
    Package pkg;
    Path pkgpath("./temp/x.tcf");
    pkg.createNew(pkgpath);
    pkg.createDir("testdir1", "*/");
}

int main() 
{    
    InitLogger();
    slog_info(g_AppLogger) << "enter main..." << endl;
    TestCase_TestPackage();
    return 0;
}

