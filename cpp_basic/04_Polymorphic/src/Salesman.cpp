#include "Salesman.h"
Salesman::Salesman() : Employee()
{
	this->saleAmount = 0.0;
}
Salesman::~Salesman()
{
}
void Salesman::initialization()
{
	cout << "请输入销售员姓名" << endl;
	cin >> name;
	cout << "请输入销售员岗位等级" << endl;
	cin >> level;
	if (level > 3 || level < 0)
	{
		cout << "销售员的等级为1~3,请确认您的输入" << endl;
		cin >> level;
	}
}
void Salesman::monthSalary()
{
	if (saleAmount == 0.0)
	{
		cout << "请输入销售员的本月销售额" << endl;
		cin >> saleAmount;
	}

	int baseSalary = 0;
	switch (level)
	{
	case 1:
		baseSalary = 3000;
		break;
	case 2:
		baseSalary = 5000;
		break;
	case 3:
		baseSalary = 8000;
		break;

	default:
		cout << "销售员的等级为1~3,请确认您的输入" << endl;
		break;
	}
	salary = baseSalary + saleAmount * 0.05;
	cout << name << ":本月总工资为：" << salary << endl;
}
void Salesman::changeLevel()
{
	cout << "请输入 " << name << " 新的岗位等级" << endl;
	cin >> level;
	if (level > 3)
	{
		cout << "销售员的等级为1~3,请确认您的输入" << endl;
		cin >> level;
	}
	Salesman::monthSalary();
	cout << name << ":新的岗位等级为" << level << endl;
}
double Salesman::getSaleAmount()
{
	return saleAmount;
}