#include "standardFormatterXmlNodeParser.h"
#include "standardFormatter.h"
USING_SFLOGGER;

SFLOGGER_NAMESPACE_BEGIN
HeapObject* StandardFormatterXmlNodeParser::parse(my_xml_node& cls_node)
{
    return new StandardFormatter();
}

StandardFormatterXmlNodeParser::StandardFormatterXmlNodeParser()
{

}

StandardFormatterXmlNodeParser::~StandardFormatterXmlNodeParser()
{

}
NAMESPACE_END

