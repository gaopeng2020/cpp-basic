#include "MyArray.hpp"

using namespace ::std;
/*
泛型(Generic Programming)是指能够在多种数据类型上皆可操作的含意；
而泛型的支持机制就是模板(Templates)，把一个原本特定于某个类型的算法或类当中的类型信息抽掉,
换成模板参数T。
C++泛型主要有两种：主要有模板函数，和模板类
*/

void gSwapTest()
{
    int i1 = 10, i2 = 20;
    gSwap<int>(i1, i2);
    cout << i1 << "\t" << i2 << endl;

    double f1 = 10.1, f2 = 19.9;
    gSwap<double>(f1, f2);
    cout << f1 << "\t" << f2 << endl;

    string str1 = "Hello", str2 = "C++";
    gSwap<string>(str1, str2);
    cout << str1 << "\t" << str2 << endl;
}

void myArrayTest()
{
    MyArray<int> array(10);
    int a1 = 10, a2 = 20, a3 = 30;
    array.push(a1), array.push(a2), array.push(a3);
    array.push(40), array.push(50), array.push(60);
    array[6] = 70;

    cout << array << endl;
    cout << sizeof(array) << endl;
}

int main()
{
    gSwapTest();

    myArrayTest();
}
