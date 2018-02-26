#include "xmlClsNodeParserLodge.h"

SFLOGGER_NAMESPACE_BEGIN

XmlClsNodeParserLodge& XmlClsNodeParserLodge::GetInst()
{
    static XmlClsNodeParserLodge inst;
    return inst;
}

void XmlClsNodeParserLodge::join(const std::xStrT& clsname, XmlClsNodeParser& parser)
{
    Name2ParserMapType::const_iterator iter = parser_map_.find(clsname);
    if (iter == parser_map_.end())
        parser_map_[clsname] = &parser;
}

XmlClsNodeParser* XmlClsNodeParserLodge::getClsNodeParser(const std::xStrT& clsname)
{
    Name2ParserMapType::const_iterator iter = parser_map_.find(clsname);    
    return (iter == parser_map_.end() ? NULL : (*iter).second);
}

XmlClsNodeParserLodge::XmlClsNodeParserLodge()
{
}

XmlClsNodeParserLodge::~XmlClsNodeParserLodge()
{
}

NAMESPACE_END
