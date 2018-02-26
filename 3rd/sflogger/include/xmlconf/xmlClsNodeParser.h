#ifndef __CLS_NODE_PARSER_H__
#define __CLS_NODE_PARSER_H__

#include "xmlconfpublic.h"
#include "rapidxml.hpp"

SFLOGGER_NAMESPACE_BEGIN
class SFLOGGER_API XmlClsNodeParser
{
public:      
    typedef rapidxml::xml_node<xCharT> my_xml_node;
    typedef rapidxml::xml_attribute<xCharT> my_xml_attribute;    
    virtual SFLOGGER::HeapObject* parse(my_xml_node& cls_node) = 0;
    virtual ~XmlClsNodeParser() {}
};
NAMESPACE_END

#endif