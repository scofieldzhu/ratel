#include <iostream>
#include "person.h"
#include "archive.h"
USING_RATEL

void WriteTestData()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, true);
	Person p1("JiYouWen");
	p1.age = 33;
	ar << p1;
}

void ReadTestData()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, false);
	SClsMeta* m = ar.readClsMeta();
	if(m){
		Person* p = Person::SafeCast(ar.readObject(*m));
		if(p)
			p->print();
	}
}

int main()
{
	WriteTestData();
	ReadTestData();
	return 0;
}
