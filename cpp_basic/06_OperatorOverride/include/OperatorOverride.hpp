#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <math.h>
#include <string>
using namespace std;

template <typename T>
class OperatorOverride;

//操作符<< 或者 >>必须在通过友元函数在类外声明
//在模板类中，对于友元重载操作符 << 或者 >> 需要在	operator<</>>和参数列表之间加⼊<T>
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
	//移位运算符重载 只必须在全局中声明， 如果写在成员中调用顺序就反了，如 another<<cout
	//友元不是类成员,但是它可以访问类中的私有成员。友元的作用在于提高程序的运行效率,
	//但是, 它破坏了类的封装性和隐藏性, 使得非成员函数可以访问类的私有成员
	friend ostream &operator<<<T>(ostream &os, OperatorOverride<T> &another);
	friend istream &operator>><T>(istream &is, OperatorOverride<T> &another);
};

//模板类的实现必须在函数前一行加上template <typename T>，且函数作用域用xxxclass<T>修饰；
// 模版类的定义和实现分开写了，编译将会出错。
//调用模板类时必须引入模板类实现文件.cpp，为了区分一般将模板类实现文件后缀改为.hpp
/**************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Template Class Definition >>          DO NOT CHANGE THIS COMMENT!
 **************************************************************************************************************/
template <typename T>
OperatorOverride<T>::OperatorOverride()
{
	this->real = 0;
	this->imaginary = 0;
}

template <typename T>
OperatorOverride<T>::OperatorOverride(T real, T imaginary)
{
	this->real = real;
	this->imaginary = imaginary;
}

template <typename T>
string OperatorOverride<T>::operator()(T real, T imaginary)
{
	//字符串和数字拼接
#if 0
	//C格式
	char* buffer = (char*)malloc(13);
	sprintf(buffer, "%d%s%d%s", real, " + ", imaginary, "i");
	return buffer;
#endif

#if 1
	return to_string(real) + " + " + to_string(imaginary) + "i";
#endif // 0
}

template <typename T>
OperatorOverride<T>::~OperatorOverride()
{
}

template <typename T>
void OperatorOverride<T>::toString()
{
	cout << "(" << real << "+ " << imaginary << "i )" << endl;
}

template <typename T>
OperatorOverride<T> &OperatorOverride<T>::operator++()
{
	this->real++;
	this->imaginary++;
	return *this;
}

template <typename T>
const OperatorOverride<T> OperatorOverride<T>::operator++(int)
{
	this->real++;
	this->imaginary++;
	return *this;
}

template <typename T>
OperatorOverride<T> &OperatorOverride<T>::operator--()
{
	this->real--;
	this->imaginary--;
	return *this;
}

template <typename T>
OperatorOverride<T> OperatorOverride<T>::operator+(OperatorOverride<T> &another)
{
	//z1 + z2 = (x1 + x2)+i(y1 + y2)
	OperatorOverride temp(this->real + another.real, this->imaginary + another.imaginary);
	return temp;
}

template <typename T>
OperatorOverride<T> OperatorOverride<T>::operator-(OperatorOverride<T> &another)
{
	//z1 - z2 = (x1 - x2)+i(y1 - y2)
	OperatorOverride temp(this->real - another.real, this->imaginary - another.imaginary);
	return temp;
}

template <typename T>
OperatorOverride<T> OperatorOverride<T>::operator*(OperatorOverride<T> &another)
{
	//z1 * z2 = (x1*x2 + y1*y2)+i(x1*y2 + x2*y1)
	OperatorOverride temp;
	temp.real = this->real * another.real - this->imaginary * another.imaginary;
	temp.imaginary = this->imaginary * another.imaginary + another.real * this->imaginary;
	return temp;
}

template <typename T>
OperatorOverride<T> OperatorOverride<T>::operator/(OperatorOverride<T> &another)
{
	//z1 / z2 = (x1*x2 + y1*y2)/(x2^2 + y2^2)+i(x2*y1 - x1*y2)/(x2^2 + y2^2)
	OperatorOverride temp;

	temp.real = (this->real * another.real + this->imaginary * another.imaginary) / (int)(pow(another.real, 2) + pow(another.imaginary, 2));
	//+ (this->real * another.real + this->imaginary * another.imaginary) % (another.real ^ 2 + another.imaginary ^ 2);

	temp.imaginary = (another.real * this->imaginary - this->real * another.imaginary) / (int)(pow(another.real, 2) + pow(another.imaginary, 2));
	//+ (another.real * this->imaginary - this->real * another.imaginary) % (another.real ^ 2 + another.imaginary ^ 2);

	return temp;
}

template <typename T>
OperatorOverride<T> OperatorOverride<T>::operator+=(OperatorOverride<T> &another)
{
	OperatorOverride temp(this->real += another.real, this->imaginary += another.imaginary);
	return temp;
}

template <typename T>
OperatorOverride<T> OperatorOverride<T>::operator-=(OperatorOverride<T> &another)
{
	OperatorOverride temp(this->real -= another.real, this->imaginary -= another.imaginary);
	return temp;
}

template <typename T>
ostream &operator<<(ostream &os, OperatorOverride<T> &another)
{
	os << "( " << another.real << "+" << another.imaginary << ")" << endl;
	return os;
}

template <typename T>
istream &operator>>(istream &is, OperatorOverride<T> &another)
{
	cout << "real = ";
	is >> another.real;
	cout << "imaginary = ";
	is >> another.imaginary;
	return is;
}
