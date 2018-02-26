#include "xmlConfParseException.h"

SFLOGGER_NAMESPACE_BEGIN
const std::xStrT& XmlConfParseException::what_err() const
{
    return msg_;
}

XmlConfParseException::XmlConfParseException(const xCharT* msg)
    :std::exception(NULL),
    msg_(msg)
{
}

XmlConfParseException::~XmlConfParseException()
{
}
NAMESPACE_END
