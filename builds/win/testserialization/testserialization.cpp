#include <iostream>
#include "person.h"
#include "archive.h"
#include "sobjectlist.h"
USING_RATEL

void WriteTestData()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, true);	

	Person* p1 = new Person("JiYouWen");
	p1->age = 33;

	Person* p2 = new Person("Wangdaoliang");
	p2->age = 34;
	p2->salary = 2300.8;

	SObjectList objects;
	objects.append(SObjectSPtr(p1));
	objects.append(SObjectSPtr(p2));
	objects.serialize(ar);	
}

void ReadTestData()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, false);
	SObjectList objs;
	objs.serialize(ar);
	if(!objs.empty()){
		for(uint32_t i = 0; i < objs.count(); ++i){
			Person* p = Person::SafeCast(objs.object(i).get());
			p->print();
		}
	}
}

int main()
{
	WriteTestData();
	ReadTestData();
	return 0;
}
