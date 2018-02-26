#include "XmlConfParser.h"
#include "xmlConfParseException.h"
#include "xmlParserHelper.h"
#include "xmlClsNodeParser.h"
#include "xmlClsNodeParserLodge.h"
#include "loggerMgr.h"
#include "xtext.h"
#include "logger.h"
#include <iostream>
using namespace std;
using namespace rapidxml;
USING_SFLOGGER;

#define SAFE_RELEASE_FILE_BUFFER \
    if(file_content_buffer != NULL) \
    { \
        delete[] file_content_buffer; \
        file_content_buffer = NULL; \
    }        

SFLOGGER_NAMESPACE_BEGIN
typedef xml_document<xCharT> my_xml_document;
typedef XmlClsNodeParser::my_xml_node my_xml_node;
typedef XmlClsNodeParser::my_xml_attribute my_xml_attribute;

struct XmlConfParser::Impl 
{
    Logger* parseLoggerNode(my_xml_node& root_node) throw (XmlConfParseException&);    
    void readutf8ConfContent(const xStrT& conf_file);
    void releaseResource();
    Impl();
    ~Impl();
    my_xml_document doc_;    
    xCharT* conf_filecontent_;    
};

Logger* XmlConfParser::Impl::parseLoggerNode(my_xml_node& root_node) throw (XmlConfParseException&)
{
    my_xml_attribute* cls_attr = root_node.first_attribute(LotsOfKeyAttrs::CLS_ATTR);
    COND_VERIFYEX(cls_attr != NULL, _X("%s Attribute Of Logger Xml Node Not Found!"), LotsOfKeyAttrs::CLS_ATTR);
    const xStrT cls_name(cls_attr->value());
    XmlClsNodeParser* right_parser = XmlClsNodeParserLodge::GetInst().getClsNodeParser(cls_name);
    return (right_parser != NULL ? dynamic_cast<Logger*>(right_parser->parse(root_node)) : NULL);
}

void XmlConfParser::Impl::readutf8ConfContent(const xStrT& conf_file)
{    
    FILE* fp = NULL;
    errno_t error = xfopen(&fp, conf_file.c_str(), _X("rb,ccs=UTF-8"));
    COND_VERIFYEX(error == 0, _X("%s not exists!\r\n"), conf_file.c_str());
    fseek(fp, 0, SEEK_END);     
    const int CONTENT_BUFFER_SIZE = ftell(fp) - 3; 
    rewind(fp);        
    char* raw_content_buffer = new char[CONTENT_BUFFER_SIZE + 1];    
    fread(raw_content_buffer, 1, 3, fp);//jump over file BOM:\xef\xbb\xbf
    const size_t numberOfItemsFinished = fread(raw_content_buffer, 1, CONTENT_BUFFER_SIZE, fp);
    COND_VERIFY(numberOfItemsFinished == CONTENT_BUFFER_SIZE, _X("read conf file failed!"));        
    raw_content_buffer[CONTENT_BUFFER_SIZE] = '\0';
    Utf8ToWideChar(raw_content_buffer, conf_filecontent_);
    fclose(fp);
    delete[] raw_content_buffer;
}

XmlConfParser::Impl::Impl()
    :conf_filecontent_(NULL)    
{
}

XmlConfParser::Impl::~Impl()
{
    releaseResource();
}

void XmlConfParser::Impl::releaseResource()
{
    if (conf_filecontent_)
        delete[] conf_filecontent_;
    conf_filecontent_ = NULL;    
}

void XmlConfParser::parse() throw (XmlConfParseException&)
{
    my_xml_node* root_node = impl_->doc_.first_node(LotsOfKeyNodes::ROOT);
    COND_VERIFYEX(root_node != NULL, _X("%s Node Not Found!"), LotsOfKeyNodes::ROOT);
    my_xml_node* logger_node = root_node->first_node(LotsOfKeyNodes::LOGGER);
    while (logger_node)
    {
        Logger* logger = impl_->parseLoggerNode(*logger_node);
        COND_VERIFYEX(logger != NULL, _X("LoggerXmlNode Parsed Error!\r\n"));
        LoggerMgr::GetInst()->addLogger(*logger);
        logger_node = logger_node->next_sibling(LotsOfKeyNodes::LOGGER);
    }
}

XmlConfParser::XmlConfParser(const xCharT* conf_file)
    :impl_(new Impl)
{    
    try
    {                        
        impl_->readutf8ConfContent(conf_file);
        impl_->doc_.parse<0>(impl_->conf_filecontent_);        
    }
    catch (XmlConfParseException& e)
    {
        impl_->releaseResource();
        throw e;
    }
    catch (parse_error& e)
    {        
        impl_->releaseResource();        
        xStrT err_details = AnsiToUnicode(e.what());
        throw XmlConfParseException(err_details.c_str());
    }    
    catch (exception& e)
    {
        impl_->releaseResource();
        xStrT err_details = AnsiToUnicode(e.what());
        throw XmlConfParseException(err_details.c_str());
    }
    catch (...)
    {
        impl_->releaseResource();
        throw XmlConfParseException(_X("Unknown Exception!"));
    }    
}

XmlConfParser::~XmlConfParser()
{
}
NAMESPACE_END
