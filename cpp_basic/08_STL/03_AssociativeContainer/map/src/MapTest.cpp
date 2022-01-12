#include "MapTest.h"

void pairConstructTest()
{
    // 调用构造函数 1，也就是默认构造函数
    pair<string, double> pair1;
    pair1.first = "pair1";
    pair1.second = 0.0;

    // 调用第 2 种构造函数
    pair<string, string> pair2("pair2", "pair构造函数方式2");

    // 调用拷贝构造函数
    pair<string, string> pair3(pair2);

    //调用移动构造函数 : 推荐
    pair<string, int> pair4(make_pair("C++", 4));

    // 调用第 5 种构造函数
    pair<string, float> pair5(string("Java"), 5);

    cout << "pair1: " << pair1.first << " " << pair1.second << endl;
    cout << "pair2: " << pair2.first << " " << pair2.second << endl;
    cout << "pair3: " << pair3.first << " " << pair3.second << endl;
    cout << "pair4: " << pair4.first << " " << pair4.second << endl;
    cout << "pair5: " << pair5.first << " " << pair5.second << endl;
}
