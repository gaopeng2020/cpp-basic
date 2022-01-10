#include "StringTest.h"

void constructTest()
{
    string str1;
    str1 = "通过一个空字符串构造。";
    cout << str1 << endl;

    string str2(str1);
    str2.append("使用一个string对象初始化另一个string对象");
    cout << str2 << endl;

    const char *charPtrStr = "Hello charPtrStr";
    string str3(charPtrStr);
    cout << str3 << endl;

    const char charArrayStr[64] = "hello charArrayStr";
    string str4(charArrayStr);
    cout << str4 << endl;

    string str5(10, 's');
    cout << str5 << endl;
}

void addElementsTest()
{
    string str1 = "string& operator=(const char* s);//char*类型字符串 赋值给当前的字符串";
    cout << "str1 = " << str1 << endl;

    string str2 = str1;

    cout<<str1.compare(str2)<<endl;
    if (str1==str2)
    {
        cout<<"str1==str2"<<endl;
    }

    string str3;
    str3.assign(str2, 33);
    cout << "str3 = " << str3 << endl;

    cout << str3[0] << endl;
    cout << str3.at(2) << endl;

    str3.insert(str3.length(), str2);
    cout << "str3 = " << str3 << endl;
}

void subStringTest()
{
    string str = "String.find() Test";
    size_t findPos = str.find("find");
    if (findPos != string::npos)
    {
        cout << "findPos = " << findPos << endl;
        cout << "findPosAfter = " << str.substr(findPos) << endl;

        cout << "findPosBefore = " << str.substr(0,findPos-1)<<endl;
    }
}

void stringParse(){
    string stri = "0xFF";
    int i = stoi(stri,0,16);
    cout<<"i = "<<i<<endl;

    string strd = "3.141592653587932";
    long double ld = stold(strd);
    cout <<"long double ld = "<<ld<<endl;

    string str = to_string(ld);
    cout<<"str = "<<str<<endl;

}