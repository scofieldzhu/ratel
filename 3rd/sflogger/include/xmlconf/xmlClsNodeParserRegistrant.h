#ifndef __CLS_NODE_PARSER_REGISTRANT_H__
#define __CLS_NODE_PARSER_REGISTRANT_H__

#include "xmlconfpublic.h"

SFLOGGER_NAMESPACE_BEGIN
class SFLOGGER_API XmlClsNodeParserRegistrant 
{
public:
    virtual void registerAllParsers();
    XmlClsNodeParserRegistrant();
    virtual ~XmlClsNodeParserRegistrant();
};
NAMESPACE_END

#endif