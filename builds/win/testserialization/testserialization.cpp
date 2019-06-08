#include <iostream>
#include "student.h"
#include "teacher.h"
#include "class.h"
#include "archive.h"
#include "sobjectlist.h"
USING_RATEL

void WriteTestData()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, true);	

	Class* newclass = new Class("ComputeTechnolog");

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

	newclass->students.append(SObjectSPtr(p1));
	newclass->students.append(SObjectSPtr(p2));
	newclass->students.append(SObjectSPtr(p3));

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

	newclass->teachers.append(SObjectSPtr(t1));
	newclass->teachers.append(SObjectSPtr(t2));

	newclass->serialize(ar);
}

void ReadTestData()
{
	std::wstring filename = L"testserialization.bin";
	Archive ar(filename, false);
	Class* dbclass = new Class();
	dbclass->serialize(ar);
	dbclass->print();
	delete dbclass;
}

int main()
{
	WriteTestData();
	ReadTestData();
	return 0;
}
