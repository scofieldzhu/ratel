#include "xmlClsNodeParserRegistrant.h"
#include "xmlClsNodeParserLodge.h"
#include "levelFilterXmlNodeParser.h"
#include "loggerXmlNodeParser.h"
#include "consoleAppenderXmlNodeParser.h"
#include "fileAppenderXmlNodeParser.h"
#include "standardFormatterXmlNodeParser.h"
#include "sqlite3AppenderXmlNodeParser.h"

SFLOGGER_NAMESPACE_BEGIN

void XmlClsNodeParserRegistrant::registerAllParsers()
{
    JOIN_PARSER_LODGE(LevelFilter, LevelFilterXmlNodeParser)
    JOIN_PARSER_LODGE(Logger, LoggerXmlNodeParser)
    JOIN_PARSER_LODGE(ConsoleAppender, ConsoleAppenderXmlNodeParser)
    JOIN_PARSER_LODGE(FileAppender, FileAppenderXmlNodeParser)
    JOIN_PARSER_LODGE(StandardFormatter, StandardFormatterXmlNodeParser)
    JOIN_PARSER_LODGE(Sqlite3Appender, Sqlite3AppenderXmlNodeParser)
}

XmlClsNodeParserRegistrant::XmlClsNodeParserRegistrant()
{

}

XmlClsNodeParserRegistrant::~XmlClsNodeParserRegistrant()
{

}
NAMESPACE_END
