#ifndef __CONF_XML_PARSER_H__
#define __CONF_XML_PARSER_H__

#include "xmlconfpublic.h"

SFLOGGER_NAMESPACE_BEGIN
class SFLOGGER_API XmlConfParser
{
public:    
    void parse() throw (XmlConfParseException&);
    XmlConfParser(const xCharT* conf_file);
    ~XmlConfParser();
private:
    struct Impl;
    std::auto_ptr<Impl> impl_;
};
NAMESPACE_END

#endif