#include "sqlite3AppenderXmlNodeParser.h"
#include "sqlite3Appender.h"
#include "xmlConfParseException.h"
#include "xmlParserHelper.h"
using namespace std;

SFLOGGER_NAMESPACE_BEGIN
namespace {
    const xCharT* kDBFilePathNodeName = _X("db_filepath");    
}
HeapObject* Sqlite3AppenderXmlNodeParser::parse(my_xml_node& cls_node)
{
    my_xml_node* filepath_node = cls_node.first_node(kDBFilePathNodeName);
    COND_VERIFYEX(filepath_node != NULL, _X("%s Child Node Of Sqlite3Appender Not Found!\r\n"), kDBFilePathNodeName);
    const xStrT db_filepath = filepath_node->value();
    const int flush_frequence = parseFlushFrequnce(cls_node);
    Sqlite3Appender* db_appender = new Sqlite3Appender(db_filepath, flush_frequence);
    my_xml_node* filter_node = cls_node.first_node(LotsOfKeyNodes::FILTER);
    if (filter_node)
        db_appender->setFilter(parseFilter(*filter_node));
    my_xml_node* formatter_node = cls_node.first_node(LotsOfKeyNodes::FORMATTER);
    if (formatter_node)
        db_appender->setFormatter(parserFormatter(*formatter_node));
    return db_appender;
}

Sqlite3AppenderXmlNodeParser::Sqlite3AppenderXmlNodeParser()
{

}

Sqlite3AppenderXmlNodeParser::~Sqlite3AppenderXmlNodeParser()
{

}

NAMESPACE_END
