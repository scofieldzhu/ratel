#include <iostream>
#include "student.h"
#include "teacher.h"
#include "archive.h"
#include "sobjectlist.h"
USING_RATEL

void WriteTestData()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, true);	

	Student* p1 = new Student("JiYouWen");
	p1->age = 33;

	Student* p2 = new Student("Wangdaoliang");
	p2->sid = "S1103101134";
	p2->age = 34;
	p2->gender = 'f';
	p2->score = 85.0;

	Student* p3 = new Student("zhuchengguang");
	p3->sid = "S1103101125";
	p3->age = 35;
	p3->gender = 'm';
	p3->score = 86.5;

	SObjectList students;
	students.append(SObjectSPtr(p1));
	students.append(SObjectSPtr(p2));
	students.append(SObjectSPtr(p3));
	students.serialize(ar);	

	Teacher* t1 = new Teacher("Wangzheng");
	t1->age = 45;
	t1->tid = "T1103101126";
	t1->gender = 'm';
	t1->salary = 24000.3;

	Teacher* t2 = new Teacher("Jijuan");
	t2->age = 35;
	t2->tid = "T1103101127";
	t2->gender = 'f';
	t2->salary = 20000.3;

	SObjectList teachers;
	teachers.append(SObjectSPtr(t1));
	teachers.append(SObjectSPtr(t2));
	teachers.serialize(ar);
}

void ReadTestData()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, false);
	SObjectList students;
	students.serialize(ar);
	if(!students.empty()){
		for(uint32_t i = 0; i < students.count(); ++i){
			SObject* so = students.object(i).get();
			Person* p = Person::SafeCast(so);
			p->print();
		}
	}

	SObjectList teachers;
	teachers.serialize(ar);
	if (!teachers.empty()){
		for (uint32_t i = 0; i < teachers.count(); ++i){
			Person* p = Person::SafeCast(teachers.object(i).get());
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
