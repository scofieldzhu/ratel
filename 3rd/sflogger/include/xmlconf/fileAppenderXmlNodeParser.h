#ifndef __FILE_APPENDER_NODE_PARSER_H__
#define __FILE_APPENDER_NODE_PARSER_H__

#include "baseAppenderXmlNodeParser.h"

SFLOGGER_NAMESPACE_BEGIN
class SFLOGGER_API FileAppenderXmlNodeParser : public BaseAppenderXmlNodeParser
{
public:
    HeapObject* parse(my_xml_node& cls_node);
    FileAppenderXmlNodeParser();
    ~FileAppenderXmlNodeParser();
};
NAMESPACE_END
#endif