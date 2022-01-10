#include "Employee.h"
#include "Engineer.h"
#include "Manager.h"
#include "Salesman.h"
#include "SalseManager.h"
/*
多态(polymorphism):由继承而产生的相关的不同的类,其对象对同一消息会作出不同的响应。
赋值兼容规则:指在需要基类对象的任何地方都可以使用公有派生类的对象来替代。
	1. 派生类的对象可以赋值给基类对象。
	2. 派生类的对象可以初始化基类的引用。
	3. 派生类对象的地址可以赋给指向基类的指针。

如果父类指针指向的是父类对象则调用父类中定义的函数；
如果父类指针指向的是子类对象则调用子类中定义的重写函数；

C++中通过virtual关键字对多态进行支持,使用virtual声明的函数被重写后即可展现多态特性.但虚函数的效率要低很多。
多态成立的条件:
	1. 要有继承
	2. 要有虚函数重写
	3. 要有父类指针（父类引用）指向子类对象

构造函数不能是虚函数。析构函数可以是虚的。虚析构函数用于指引delete运算符正确析构动态对象.

重载（添加） ：
	a. 相同的范围（在同一个类中）
	b. 函数名字相同
	c. 参数不同
	d. virtual关键字可有可无
重写（覆盖) 是指派生类函数覆盖基类函数，特征是：
	a. 不同的范围，分别位于基类和派生类中
	b. 函数的名字相同
	c. 参数相同
	d. 基类函数必须有virtual关键字
重定义(隐藏) 是指派生类的函数屏蔽了与其同名的基类函数，规则如下：
	a. 如果派生类的函数和基类的函数同名，但是参数不同，此时，不管有无virtual，基类的函数被隐藏。
	b. 如果派生类的函数与基类的函数同名，并且参数也相同，但是基类函数没有visual关键字，此时，基类的函数被隐藏。

纯虚函数：在基类中只有声明没有定义，要求任何派生类都定义自己的版本，纯虚函数为个派生类提供一个公共接口。
	virtual 类型 函数名（参数表）= 0;
	含有纯虚函数的类,称为抽象基类,不可实列化，,如果一个类中声明了纯虚函数,而在派生类中没有对该函数定义,派生类仍然为纯虚基类。
	C++中没有接口的概念,C++中可以使用纯虚函数实现接口的功能。
*/

int main()
{
	//Employee* employee = new Engineer();

	//Employee* employee = new Manager();

	//Employee* employee = new Salesman();
	// std:
	// ostream:
	// 	cout << "############################################" << endl;
	Employee *employee = new SalseManager();

	employee->initialization();
	employee->monthSalary();
	employee->employeeInfo();
	employee->changeLevel();
	employee->employeeInfo();
}
