#include <iostream>
#include <windows.h>
#include "package.h"
#include "path.h"
#include "defAppender.h"
#include "logger.h"
#include "standardFormatter.h"
#include "sqlitelogger.h"
#include "pkglogger.h"

#include "pathremover.h"
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

void TestCase_TestPackage_Create()
{
    Package pkg("./temp");
    Path pkgpath("./temp/y.tcf");
    if (!pkg.createNew(pkgpath))
        return;
    if (!pkg.createDir("testdir1", "*/")) 
        return;    
    if (!pkg.createDir("testdir11", "*/testdir1"))
        return;
    if(!pkg.createDir("testdir2", "*/"))
        return;
    if(!pkg.importDir("*/testdir2", "C:\\work\\github\\ratel\\ratel\\src"))
        return; 
    pkg.commit();

}

void TestCase_TestPackage_Load()
{
    Package pkg("./temp");
    if(!pkg.load("y.tcf")){
        slog_err(g_AppLogger) << "load package failed!" << endl;
        return;
    }
    slog_err(g_AppLogger) << "load package ok!" << endl;


}

int main() 
{    
    InitLogger();
    slog_info(g_AppLogger) << "enter main..." << endl;

    //TestCase_TestPackage_Create();
    TestCase_TestPackage_Load();


    return 0;
}

