#pragma once
#include <iostream>
#include <string>
using namespace std;

/* 构造函数
string();//创建一个空的字符串 
string(const string& str);//使用一个string对象初始化另一个string对象
string(const char* s);//使用字符串s初始化
string(int n,char c);//使用n个字符c初始化 */

/* 基本赋值操作
string& operator=(const char* s);//char*类型字符串 赋值给当前的字符串
string& operator=(const string &s);//把字符串s赋值给当前的字符串
string& operator=(char c);//字符赋值给当前的字符串
string& assign(const char* s);//把字符串s赋值给当前的字符串
string& assign(const char* s,int n);//把字符串s的前n个字符赋值给当前的字符串
string& assign(const string &s);//把字符串s赋值给当前的字符串
string& assign(int n,char c);//用n个字符c赋值给当前字符串
string& assign(const string &s,int start,int n);//将s从start开始n个字符赋值给字符串 */

/* 存取字符操作
char& operator[](int n);//通过【】方式取字符
char& at(int n);//通过at方法获取字符 */

/* 拼接操作
string& operator+=(const string& str);//重载+=运算符
string& operator+=(const char* str);//重载+=运算符
string& operator+=(const char c);//重载+=运算符
string& append(const char *s);//把字符串s连接到当前字符串结尾
string& append(const char *s,int n);//把字符串s的前n个字符连接到当前字符串结尾
string& append(const string &s);//同operator+=（）
string& append(const string &s,int pos,int n);//把字符串s中从pos开始的n个字符连接到当前字符串结尾
string& append( int n,char c);//在当前字符串结尾添加n个字符c */

/* 查找和替换
int find(const string& str,int pos = 0)const;//查找str第一次出现的位置，从pos开始查找
int find(const char* s,int pos = 0)const;//查找s第一次出现位置，从pos开始查找
int find(const char *s,int pos,int n)const;//从pos位置查找s的前n个字符第一次位置
int find(const char c,int pos = 0)const;//查找字符c第一次出现位置
int rfind(const string& str, int pos = npos)const;//查找str最后一次出现位置，从pos开始查找
int rfind(const char* s,int pos = npos)const;//查找s最后一次出现位置，从pos开始查找
int rfind(const char* s,int pos,int n )const;//从pos查找s的前n个字符最后一次位置
int rfind(const char c,int pos = 0)const;//查找字符c最后一次出现位置
string& replace(int pos,int n,const string& str);//替换从pos开始的n个字符为字符串str
string& replace(int pos,int n,const char* s);//替换从pos开始的n个字符为字符串s */

/* 比较操作
int compare(const string &s)const;//与字符串s比较
int compare(const char*s)const;//与字符串s比较
//compare函数在>时返回1，<时返回-1，相等时返回0，比较区分大小写，逐个字符比较 */

/* 插入和删除操作
string& insert(int pos,const char* s);//插入字符串
string& insert(int pos,const string &str);//插入字符串
string& insert(int pos,int n,char c);//在指定位置插入n个字符c
string& erase(int pos,int n = npos);//删除从pos开始的n个字符 */

/* sting和数值之间的转换
stoi（字符串，起始位置，n进制），将 n 进制的字符串转化为十进制int
stol（字符串，起始位置，n进制），将 n 进制的字符串转化为十进制long
stoui（字符串，起始位置，n进制），将 n 进制的字符串转化为十进制unsigned long
stoll（字符串，起始位置，n进制），将 n 进制的字符串转化为十进制long long
stoull（字符串，起始位置，n进制），将 n 进制的字符串转化为十进制进制的字符串转化为十进制unsigned long long
stof（字符串，起始位置），将 n 进制的字符串转化为float
stod（字符串，起始位置），将 n 进制的字符串转化为double
stold（字符串，起始位置），将 n 进制的字符串转化为long double */



void constructTest();

void addElementsTest();

void subStringTest();

void stringParse();