#include "consoleAppenderXmlNodeParser.h"
#include "consoleAppender.h"
#include "xmlConfParseException.h"
#include "xtext.h"
USING_SFLOGGER;
using namespace std;

SFLOGGER_NAMESPACE_BEGIN
namespace {
    const xCharT* kFFNodeName = _X("flush_frequence");
}
HeapObject* ConsoleAppenderXmlNodeParser::parse(my_xml_node& cls_node)
{
    HeapObject* appender = BaseAppenderXmlNodeParser::parse(cls_node);
    if (appender == NULL)
        return NULL;
    ConsoleAppender* my_appender = dynamic_cast<ConsoleAppender*>(appender);
    my_xml_node* ffnode = cls_node.first_node(kFFNodeName);
    if (ffnode)
    {        
        int value = Str2Int(ffnode->value());
        COND_VERIFYEX(value != 0, _X("invalid %s value!\r\n"), kFFNodeName);
        my_appender->setFlushFrequence(value);
    }
    return my_appender;
}

ConsoleAppenderXmlNodeParser::ConsoleAppenderXmlNodeParser()
{

}

ConsoleAppenderXmlNodeParser::~ConsoleAppenderXmlNodeParser()
{

}

Appender* ConsoleAppenderXmlNodeParser::createAppenderInstance()
{
    return new ConsoleAppender();
}
NAMESPACE_END

