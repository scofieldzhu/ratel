#include <iostream>
#include "rstring.h"
using namespace std;
USING_RATEL;

void TestRString_ConvNum()
{
	RString xx = "16";	
	xx = "1635";
	int16_t xxi16 = xx.toInt16();
	xx = "256544555";
	int32_t xxi32 = xx.toInt32();
	xx = "123456988455555";
	int64_t xxi64 = xx.toInt64();
	double xxd = 125.3698;
	xx.fromDouble(xxd, 4);
	xx = "2.536442";
	double xxd1 = xx.toDouble();
	xxd1 = xxd1;
}

void TestRString_NewUID()
{
	RString uid = RString::NewUID();
	cout << uid.cstr() << endl;
	uid = RString::NewUID();
	cout << uid.cstr() << endl;
	uid = RString::NewUID();
	cout << uid.cstr() << endl;
	uid = RString::NewUID();
	cout << uid.cstr() << endl;
}

int main()
{    
	//TestRString_ConvNum();  
	TestRString_NewUID();
    return 0;
}