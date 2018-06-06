#include <iostream>
#include "rstring.h"
using namespace std;
USING_RATEL;

int main()
{
    

    RString x = "ff";
    RStrings result =  x.split('&');
    for(int i = 0; i < result.size(); ++i)
        cout << result[i].cstr() << endl;        
    return 0;
}