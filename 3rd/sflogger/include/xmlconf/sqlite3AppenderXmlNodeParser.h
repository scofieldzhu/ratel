#ifndef __SQLITE3_APPENDER_XMLNODE_PARSER_H__
#define __SQLITE3_APPENDER_XMLNODE_PARSER_H__

#include "baseAppenderXmlNodeParser.h"

SFLOGGER_NAMESPACE_BEGIN
class SFLOGGER_API Sqlite3AppenderXmlNodeParser : public BaseAppenderXmlNodeParser
{
public:
    HeapObject* parse(my_xml_node& cls_node);
    Sqlite3AppenderXmlNodeParser();
    ~Sqlite3AppenderXmlNodeParser();
};
NAMESPACE_END
#endif