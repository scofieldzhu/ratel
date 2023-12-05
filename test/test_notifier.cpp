#include "test_notifier.h"
#include "basic/notifier.hpp"
#include "spdlog/spdlog.h"
using namespace ratel;

void TestF(int int_arg, float float_arg, std::string str_arg)
{
	spdlog::info("TestF trigger... int_arg:{} float_arg:{} str_arg:{}", int_arg, float_arg, str_arg);
}

void TestCase_Notifier()
{
	Notifier<int, float, std::string> nf;
	nf.bind(TestF);
	nf.invoke(10, 23.56f, "nima");
	int ix = 11;
	float fx = 12.45f;
	std::string sx = "ppx";
	nf.invokeR(ix, fx, sx);
}