#include <iostream>
#include "person.h"
#include "archive.h"
USING_RATEL

int main()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, true);

	Person p("Lilei");
	ar<<p;




    std::cout << "Hello World!\n"; 
}
