#include <iostream>
#include <windows.h>
#include "package.h"
#include "path.h"
#include "defAppender.h"
#include "logger.h"
#include "standardFormatter.h"
#include "sqlitelogger.h"
#include "pkglogger.h"

#include "pathop.h"

//#include "sqlite3.h"

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
    Package pkg("C:/work/github/ratel/ratel/builds/win/testpackage/temp");
	if(!pkg.createNew("./xx.tcf")){
		slog_err(g_AppLogger) << "createNew failed!" << endl;
		return;
	}   
	if(!pkg.createDir("testdir1", "./")){
		slog_err(g_AppLogger) << "createDir failed!" << endl;
		return;
	}
	if (!pkg.createDir("testdir2", "./")){
		slog_err(g_AppLogger) << "createDir failed!" << endl;
		return;
	}
	if (!pkg.createDir("testdir3", "./")){
		slog_err(g_AppLogger) << "createDir failed!" << endl;
		return;
	}
	if(!pkg.createDir("test31", "./testdir3")){
		slog_err(g_AppLogger) << "createDir failed!" << endl;
		return;
	}
	if(!pkg.createDir("test31", "./testdir3")){
		slog_err(g_AppLogger) << "createDir same directory failed!" << endl;
	}
	if(!pkg.importFile("./testdir3/test31", "c:/TDownload/hihi+song+mymo.mp3")){
		slog_err(g_AppLogger) << "importFile hihi+song+mymo.mp3 failed!" << endl;
		return;
	}
	if(!pkg.importDir("./testdir3", "C:/Qt/Qt5.6.1/Examples/Qt-5.6/activeqt/comapp")){
		slog_err(g_AppLogger) << "importDir C:/Qt/Qt5.6.1/Examples/Qt-5.6/activeqt/comapp failed!" << endl;
		return;
	}
// 	if(!pkg.removeFile("./testdir3/comapp/doc/src/comapp.qdoc")){
// 		slog_err(g_AppLogger) << "removeFile ./testdir3/doc/src/comapp.qdoc failed!" << endl;
// 		return;
// 	}
	pkg.commit();
    slog_info(g_AppLogger) << "load package ok!" << endl;
}

void TestCase_TestPackage_Load()
{
	Package pkg("./temp");
	if(!pkg.load("./xx.tcf")){
		slog_err(g_AppLogger) << "load package(./xx.tcf) failed!" << endl;
		return;
	}
// 	if(!pkg.exportFile("./testdir3/comapp/main.cpp", "./maincpy.cpp")){
// 		slog_err(g_AppLogger) << "export dir(./testdir3/comapp) failed!" << endl;
// 		return;
// 	}
	if(!pkg.removeFile("./testdir3/comapp/main.cpp"))
		return;
 	if(!pkg.exportDir("./testdir3/comapp", "./temp2")){
 		slog_err(g_AppLogger) << "export dir(./testdir3/comapp) failed!" << endl;
 		return;
 	}
}

int main() 
{   
	::CoInitialize(nullptr);
    InitLogger();
    slog_info(g_AppLogger) << "enter main..." << endl;

    TestCase_TestPackage_Create();
 	TestCase_TestPackage_Load();

	::CoUninitialize();

    return 0;
}

