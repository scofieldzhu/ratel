#include <iostream>
#include "test_logger.h"
#include "test_string_proxy.h"
#include "test_directed_graph.h"
#include "test_notifier.h"
using namespace std;

int main()
{
	cout << "Hello world!" << endl;
	TestCase_SPDLogger();
	//TestCase_StringProxy();
	TestCase_DirectedGraph();
	//TestCase_Notifier();
	return 0;
}