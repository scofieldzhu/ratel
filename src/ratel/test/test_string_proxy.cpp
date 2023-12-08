#include "test_string_proxy.h"
#include "ratel/basic/string_proxy.h"
using namespace ratel;

void TestCase_StringProxy()
{
	StringProxy sp;
	sp.stdStr();
	StringProxy sp0("dfsfdsf");
	std::string val = "nihaoya";
	StringProxy sp1(val);
	sp1 = sp0;
	sp1.toUpper();
	StringProxy uid = StringProxy::NewUID();
	uid = StringProxy::NewUID();
	//sp1 = "";
	//sp = "xx2";
}

