#pragma once
#include <iostream>
using namespace ::std;

//友元函数需要在模板类的开始位置做类外声明，否则编译不通过
template <class T>
class MyArray;
template <class T>
ostream &operator<<(ostream &os, MyArray<T> &array);

/* 类模板: 有多个类，其功能是相同的，仅仅是数据类型不同，所以将类中的类型进行泛化。
    语法格式：
    template<typename T>
    class ClassName
    {
        类模板主体定义
    }

    模板类派⽣类模板类：
    template <class T> 
    class childClass : public Father<T>
    {
        派⽣类主体
    }

    建议将函数体写在类内，实现模板方法建议在同一个文件.hpp中完成
    模板类方法的实现不能够用多文件编写
    模板类不要轻易写友元函数， 除了<<和>>；
    friend ostream& operator<<<T>（ostream&，自定义类&);
    ostream & operator<<(ostream & os, const Classname & obj
    {
		os<<"("<<obj._x<<","<<obj._y<<")"; 
		return os; 
    }

   friend istream& operator>><T>（istream&，自定义类&);
    istream & operator>>(istream & is, const Classname & obj)
    {
		is>>c._x>>c._y; 
		return is;
    }
    使用友元，则需要再hpp文件开头位置做如下类外声明
    template <class T> class ClassName;
    template <class T> ClassName<T> friendFunc(ClassName<T> &firstClass, ClassName<T> &another); 
	*/

/**************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Template Function Declaration >>          DO NOT CHANGE THIS COMMENT!
 **************************************************************************************************************/
template <class T>
class MyArray
{
private:
	//数组的容量
	int len;
	//数组元素的索引
	int index;
	//ptr指向指定的数组元素地址
	T *ptr;

public:
	// Template Class Declaration
	MyArray(int len);
	//MyArray ary(array)
	MyArray(const MyArray<T> &array);

	~MyArray();

	//MyArray ary=array
	MyArray<T> operator=(const MyArray<T> &array);

	//get array pointer
	T &operator[](int index);

	//MyArray ary[data]；（int data = 100）
	void push(T &data);

	//MyArray ary[100]；
	void push(T &&data);

	//重载<<运算符用于打印数组信息,
	//在模板类中 如果有友元重载操作符<<或者>>需要在operator<< 和参数列表之间加⼊<T>
	// template <class T>
	friend ostream &operator<<<T>(ostream &os, MyArray<T> &array);
};

/**************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Template Function Definition >>          DO NOT CHANGE THIS COMMENT!
 **************************************************************************************************************/
/* 函数模板，其函数类型和形参类型不具体指定，用一个虚拟的类型来代表。这个通用函数就称为函数模板。
    语法格式：
	template<typename/class T1,T2...Tn>
    返回类型 函数模板名(T1,T2...Tn)
      {
        函数模板定义体
      }
    模板函数不能自动类型转换，当函数模板被普通函数重载时,优先选择普通函数. 
	*/
template <typename T>
void gSwap(T &x, T &y)
{
	T temp = x;
	x = y;
	y = temp;
}

//普通函数，模板函数不能自动类型转换，当函数模板被普通函数重载时,优先选择普通函数.
void gSwap(int &x, int &y)
{
	int temp = x;
	x = y;
	y = temp;
}

/**************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Template Class Definition >>          DO NOT CHANGE THIS COMMENT!
 **************************************************************************************************************/
template <class T>
MyArray<T>::MyArray(int len)
{
	this->len = len;
	this->index = 0;
	//在堆上申请空间
	this->ptr = new T[this->len];
}

template <class T>
MyArray<T>::~MyArray()
{
	if (this->ptr != NULL)
	{
		delete[] this->ptr;
	}
}

//MyArray ary(array)
template <class T>
MyArray<T>::MyArray(const MyArray<T> &array)
{
	this->len = array.len;
	this->index = array.index;
	this->ptr = new T[this->len];
	for (int i = 0; i < this.lem; i++)
	{
		this->ptr[i] = array.ptr[i];
	}
}

//MyArray ary=array
template <class T>
MyArray<T> MyArray<T>::operator=(const MyArray<T> &array)
{
	if (this->ptr != NULL)
	{
		delete[] this->ptr;
	}
	this->len = array.len;
	this->index = array.index;
	this->ptr = new T[this->len];
	for (int i = 0; i < this->len; i++)
	{
		this->ptr[i] = array.ptr[i];
	}
	return *this;
}

//get array pointer
template <class T>
T &MyArray<T>::operator[](int index)
{
	return this->ptr[index];
}

//MyArray ary[data]；（int data = 100）
template <class T>
void MyArray<T>::push(T &data)
{
	if (this->index > this->len)
	{
		return;
	}
	this->ptr[index] = data;
	this->index++;
}

//MyArray ary[100]；
template <class T>
void MyArray<T>::push(T &&data)
{
	if (this->index > this->len)
	{
		return;
	}
	this->ptr[this->index] = data;
	this->index++;
}

template <class T>
ostream &operator<<(ostream &os, MyArray<T> &array)
{
	os << "(Array MaxLength = " << array.len << ", Array Size = " << array.index << ")" << endl;
	for (int i = 0; i < array.index; i++)
	{
		os << array[i] << "\t";
	}
	return os;
}