#include <iostream>
#include "person.h"
#include "archive.h"
USING_RATEL

int main()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, false);
	SClsMeta* mt = ar.readClsMeta();
	if(mt){
		SObject* so = ar.readObject(*mt);
		Person* p = dynamic_cast<Person*>(so);
		p = p;
	}

// 	Person p("Lilei");
// 	ar<<p;




    std::cout << "Hello World!\n"; 
}
