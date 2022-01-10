#include "Manager.h"
Manager::Manager() : Employee()
{
}
Manager::~Manager()
{
}
void Manager::initialization()
{
	cout << "请输入经理姓名" << endl;
	cin >> name;
	cout << "请输入经理岗位等级" << endl;
	cin >> level;
	if (level > 5)
	{
		cout << "经理的等级为1~5,请确认您的输入" << endl;
		cin >> level;
	}
}
void Manager::monthSalary()
{
	Manager::getBaseSalary();
	cout << name << ":本月总工资为：" << baseSalary << endl;
}
void Manager::changeLevel()
{
	cout << "请输入 " << name << " 新的岗位等级" << endl;
	cin >> level;
	if (level > 5)
	{
		cout << "经理的等级为1~5,请确认您的输入" << endl;
		cin >> level;
	}
	Manager::monthSalary();
	cout << name << ":新的岗位等级为" << level << endl;
}

void Manager::getBaseSalary()
{

	switch (level)
	{
	case 1:
		baseSalary = 15000;
		break;
	case 2:
		baseSalary = 20000;
		break;
	case 3:
		baseSalary = 28000;
		break;
	case 4:
		baseSalary = 38000;
		break;
	case 5:
		baseSalary = 50000;
		break;
	default:
		cout << "经理的等级为1~5,请确认您的输入" << endl;
		break;
	}
}
