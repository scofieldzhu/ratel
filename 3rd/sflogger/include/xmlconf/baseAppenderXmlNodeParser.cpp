#include "baseAppenderXmlNodeParser.h"
#include "xmlParserHelper.h"
#include "xmlConfParseException.h"
#include "xmlClsNodeParserLodge.h"
#include "filter.h"
#include "formatter.h"
#include "appender.h"

SFLOGGER_NAMESPACE_BEGIN
namespace {
    const xCharT* kFFNodeName = _X("flush_frequence");
}
HeapObject* BaseAppenderXmlNodeParser::parse(my_xml_node& cls_node)
{
    Appender* appender_inst = createAppenderInstance();
    COND_VERIFYEX(appender_inst != NULL, _X("createAppenderInstance failed!\r\n"));
    my_xml_node* filter_node = cls_node.first_node(LotsOfKeyNodes::FILTER);
    if (filter_node)    
        appender_inst->setFilter(parseFilter(*filter_node));    
    my_xml_node* formatter_node = cls_node.first_node(LotsOfKeyNodes::FORMATTER);
    if (formatter_node)    
        appender_inst->setFormatter(parserFormatter(*formatter_node));
    return appender_inst;
}

Appender* BaseAppenderXmlNodeParser::createAppenderInstance()
{
    return NULL;
}

Filter* BaseAppenderXmlNodeParser::parseFilter(my_xml_node& cls_node)
{
    my_xml_attribute* filter_cls_attr = cls_node.first_attribute(LotsOfKeyAttrs::CLS_ATTR);
    COND_VERIFYEX(filter_cls_attr != NULL, _X("%s Attr Of Filter Node Not Found!\r\n"), LotsOfKeyAttrs::CLS_ATTR);
    XmlClsNodeParser* filter_parser = XmlClsNodeParserLodge::GetInst().getClsNodeParser(filter_cls_attr->value());
    COND_VERIFYEX(filter_parser != NULL, _X("%s Parser Not Found!\r\n"), filter_cls_attr->value());
    Filter* filter = dynamic_cast<Filter*>(filter_parser->parse(cls_node));
    COND_VERIFYEX(filter != NULL, _X("%s Parse Failed!\r\n"), filter_cls_attr->value());
    return filter;
}

Formatter* BaseAppenderXmlNodeParser::parserFormatter(my_xml_node& cls_node)
{
    my_xml_attribute* formatter_cls_attr = cls_node.first_attribute(LotsOfKeyAttrs::CLS_ATTR);
    COND_VERIFYEX(formatter_cls_attr != NULL, _X("%s Attr Of Formatter Node Not Found!\r\n"), LotsOfKeyAttrs::CLS_ATTR);
    XmlClsNodeParser* formatter_parser = XmlClsNodeParserLodge::GetInst().getClsNodeParser(formatter_cls_attr->value());
    COND_VERIFYEX(formatter_parser != NULL, _X("%s Parser Not Found!\r\n"), formatter_cls_attr->value());
    Formatter* formatter = dynamic_cast<Formatter*>(formatter_parser->parse(cls_node));
    COND_VERIFYEX(formatter != NULL, _X("%s Parse Failed!\r\n"), formatter_cls_attr->value());
    return formatter;
}

int BaseAppenderXmlNodeParser::parseFlushFrequnce(my_xml_node& cls_node)
{
    my_xml_node* ffnode = cls_node.first_node(kFFNodeName);
    if (ffnode == NULL)
        return 1;    
    const int value = Str2Int(ffnode->value());
    COND_VERIFYEX(value != 0, _X("invalid %s value!\r\n"), kFFNodeName);
    return value;
}

BaseAppenderXmlNodeParser::BaseAppenderXmlNodeParser()
{

}

BaseAppenderXmlNodeParser::~BaseAppenderXmlNodeParser()
{

}
NAMESPACE_END

