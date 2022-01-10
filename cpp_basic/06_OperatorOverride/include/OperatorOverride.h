#pragma once
#include <iostream>
#include <math.h>
#include <string>
using namespace std;

template <typename T>
class OperatorOverride;

template <typename T>
ostream &operator<<(ostream &os, OperatorOverride<T> &another);
template <typename T>
istream &operator>>(istream &is, OperatorOverride<T> &another);

//运算符重载的本质是函数重载。所谓重载，就是重新赋予新的含义
//重载的运算符参数至少应有一 个是自定义类对象(或类对象的引用)
/*可重载的运算符
* 算术运算符：+ - / * % += -+ *= /= %= ++ --
* 关系运算符：> >= < <= != ==
* 逻辑运算符：&& || ！ & |
* 移位运算符：>> <<
*/

//泛型(Generic Programming)即是指具有在多种数据类型上皆可操作的含意；
//所谓函数/类模板，实际上是建立一个通用函数/类，其参类型不具体指定，用一个虚拟的类型T来代表；
//除了友元函数，其余函数在头文件中的写法保持不变；
/**************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Template Class Declaration >>          DO NOT CHANGE THIS COMMENT!
 **************************************************************************************************************/
template <typename T> //或者template<class T>
class OperatorOverride
{
private:
	T real;
	T imaginary;

public:
	OperatorOverride();					   //无参构造函数
	OperatorOverride(T real, T imaginary); //全参构造函数

	//仿函数：把类对象像函数名一样使用,在构造时就实现某个功能并将结构返回，其实现原理是重载()
	string operator()(T real, T imaginary);

	~OperatorOverride(); //析构函数

	void toString(); //提供一个打印虚数的方法

	/* 重载函数的一般格式如下 :
	函数类型 operator运算符名称(形参表列)
	{
		重载实体;
	}*/

	//单目运算符:只有一个操作bai数
	OperatorOverride &operator++(); //前++的成员函数,返回引用可以实现多次++动作，如++++++
	OperatorOverride &operator--();
	const OperatorOverride operator++(int); //后++的成员函数,加const可以限定++次数：只能++一次

	//双目运算符
	OperatorOverride operator+(OperatorOverride &another);
	OperatorOverride operator-(OperatorOverride &another);
	OperatorOverride operator*(OperatorOverride &another);
	OperatorOverride operator/(OperatorOverride &another);

	OperatorOverride operator+=(OperatorOverride &another);
	OperatorOverride operator-=(OperatorOverride &another);

	//左移与右移运算符:
	//移位运算符重载 只能写在全局中， 如果写在成员中调用顺序就反了，如 another<<cout
	//友元不是类成员,但是它可以访问类中的私有成员。友元的作用在于提高程序的运行效率,
	//但是, 它破坏了类的封装性和隐藏性, 使得非成员函数可以访问类的私有成员
	//在模板类中	对于友元重载操作符 << 或者 >> 需要在	operator<</>>和参数列表之间加⼊<T>
	friend ostream &operator<<<T>(ostream &os, OperatorOverride<T> &another);
	friend istream &operator>><T>(istream &is, OperatorOverride<T> &another);
};
