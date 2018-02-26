#ifndef __LEVEL_FILTER_NODE_PARSER_H__
#define __LEVEL_FILTER_NODE_PARSER_H__

#include "xmlClsNodeParser.h"

SFLOGGER_NAMESPACE_BEGIN
class LevelFilterXmlNodeParser : public XmlClsNodeParser
{
public:
    SFLOGGER::HeapObject* parse(my_xml_node& cls_node);
    LevelFilterXmlNodeParser();
    ~LevelFilterXmlNodeParser();
};
NAMESPACE_END
#endif