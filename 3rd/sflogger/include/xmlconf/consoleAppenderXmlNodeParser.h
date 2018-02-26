#ifndef __CONSOLE_APPENDER_NODE_PARSER_H__
#define __CONSOLE_APPENDER_NODE_PARSER_H__

#include "baseAppenderXmlNodeParser.h"

SFLOGGER_NAMESPACE_BEGIN
class SFLOGGER_API ConsoleAppenderXmlNodeParser : public BaseAppenderXmlNodeParser
{
public:
    SFLOGGER::HeapObject* parse(my_xml_node& cls_node);
    ConsoleAppenderXmlNodeParser();
    ~ConsoleAppenderXmlNodeParser();
private:
    SFLOGGER::Appender* createAppenderInstance();
};
NAMESPACE_END
#endif