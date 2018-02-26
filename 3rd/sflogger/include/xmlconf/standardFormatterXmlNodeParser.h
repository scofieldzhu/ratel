#ifndef __STANDARD_FORMATTER_NODE_PARSER_H__
#define __STANDARD_FORMATTER_NODE_PARSER_H__

#include "xmlClsNodeParser.h"

SFLOGGER_NAMESPACE_BEGIN
class SFLOGGER_API StandardFormatterXmlNodeParser : public XmlClsNodeParser
{
public:
    SFLOGGER::HeapObject* parse(my_xml_node& cls_node);
    StandardFormatterXmlNodeParser();
    ~StandardFormatterXmlNodeParser();
};
NAMESPACE_END
#endif