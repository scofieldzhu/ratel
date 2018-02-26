#include "fileAppenderXmlNodeParser.h"
#include "fileAppender.h"
#include "xmlConfParseException.h"
#include "xmlParserHelper.h"
#include "xtext.h"
USING_SFLOGGER;
using namespace std;

SFLOGGER_NAMESPACE_BEGIN
namespace {
    const xCharT* kFilePathNodeName = _X("file_path");    
}

HeapObject* FileAppenderXmlNodeParser::parse(my_xml_node& cls_node)
{    
    my_xml_node* filepath_node = cls_node.first_node(kFilePathNodeName);
    COND_VERIFYEX(filepath_node != NULL, _X("%s Child Node Of Appender Node Not Found!\r\n"), kFilePathNodeName);
    const xStrT filepath = filepath_node->value();    
    
    const int flush_frequence = parseFlushFrequnce(cls_node);

    FileAppender* file_appender = new FileAppender(filepath, flush_frequence);

    my_xml_node* filter_node = cls_node.first_node(LotsOfKeyNodes::FILTER);
    if (filter_node)
        file_appender->setFilter(parseFilter(*filter_node));

    my_xml_node* formatter_node = cls_node.first_node(LotsOfKeyNodes::FORMATTER);
    if (formatter_node)
        file_appender->setFormatter(parserFormatter(*formatter_node));

    return file_appender;
}

FileAppenderXmlNodeParser::FileAppenderXmlNodeParser()
{

}

FileAppenderXmlNodeParser::~FileAppenderXmlNodeParser()
{

}

NAMESPACE_END
