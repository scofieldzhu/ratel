#include <iostream>
#include "test_logger.h"
#include "test_string_proxy.h"
using namespace std;

int main()
{
	cout << "Hello world!" << endl;
	TestCase_SPDLogger();
	TestCase_StringProxy();
	return 0;
}