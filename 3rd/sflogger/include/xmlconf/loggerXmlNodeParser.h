#ifndef __LOGGER_CLS_NODE_PARSER_H__
#define __LOGGER_CLS_NODE_PARSER_H__

#include "xmlClsNodeParser.h"

SFLOGGER_NAMESPACE_BEGIN
class SFLOGGER_API LoggerXmlNodeParser : public XmlClsNodeParser
{
public:
    SFLOGGER::HeapObject* parse(my_xml_node& cls_node);
    LoggerXmlNodeParser();
    virtual ~LoggerXmlNodeParser();
};
NAMESPACE_END
#endif