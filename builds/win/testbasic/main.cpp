#include <iostream>
#include "arraryx.hpp"
using namespace std;
USING_RATEL;

int main()
{
    int8 x[5] = {0, 1, 2, 3, 9};
    int8 xx[5] = { 0, 11, 12, 13, 19 };
    ArrayX<int8, 6> m1 = x;
    ArrayX<int8, 6> m11 = xx;
    ArrayX<float, 5> m2;
    ArrayX<double, 6> m3;
    ArrayX<char, 6> m4;

    m1[1] = 123;

    m11 = m1;
    m11 = xx;

    m11.assign(xx);
    
    return 0;
}