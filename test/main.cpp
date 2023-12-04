#include <iostream>
#include "test_logger.h"
#include "test_string_proxy.h"
#include "test_directed_graph.h"
using namespace std;

int main()
{
	cout << "Hello world!" << endl;
	TestCase_SPDLogger();
	TestCase_StringProxy();
	TestCase_DirectedGraph();
	return 0;
}