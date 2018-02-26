#ifndef __npsdef_h__
#define __npsdef_h__

#define NAMESPACE_BEGIN(np) namespace np{
#define NAMESPACE_END }
#define USING_NAMESPACE(np) using namespace np;

#define SFLOGGER sflogger
#define SFLOGGER_NAMESPACE_BEGIN NAMESPACE_BEGIN(SFLOGGER)
#define USING_SFLOGGER USING_NAMESPACE(SFLOGGER)

#endif
