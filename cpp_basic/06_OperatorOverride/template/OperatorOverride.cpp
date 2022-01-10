#define _CRT_SECURE_NO_WARNINGS
#include <OperatorOverride.h>

//模板类的实现必须在函数前一行加上template <typename T>，且函数作用域用xxxclass<T>修饰；
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
