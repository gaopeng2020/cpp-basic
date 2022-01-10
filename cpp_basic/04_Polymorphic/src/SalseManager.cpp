#include "SalseManager.h"

SalseManager::SalseManager()
{
}
SalseManager::~SalseManager()
{
}
void SalseManager::initialization()
{
	cout << "请输入销售经理姓名" << endl;
	cin >> name;
	cout << "请输入销售经理岗位等级" << endl;
	cin >> level;
	if (level > 3 || level < 0)
	{
		cout << "销售经理的等级为1~3,请确认您的输入" << endl;
		cin >> level;
	}
}
void SalseManager::monthSalary()
{
	if (saleAmount == 0.0)
	{
		cout << "请输入销售员的本月销售额" << endl;
		cin >> saleAmount;
	}

	SalseManager::getBaseSalary();
	salary = baseSalary + saleAmount * 0.03;
	cout << name << ":本月总工资为：" << salary << endl;
}
void SalseManager::changeLevel()
{
	cout << "请输入 " << name << " 新的岗位等级" << endl;
	cin >> level;
	if (level > 3)
	{
		cout << "销售经理的等级为1~3,请确认您的输入" << endl;
		cin >> level;
	}
	SalseManager::monthSalary();
	cout << name << ":新的岗位等级为" << level << endl;
}

void SalseManager::getBaseSalary()
{
	switch (level)
	{
	case 1:
		baseSalary = 5000;
		break;
	case 2:
		baseSalary = 8000;
		break;
	case 3:
		baseSalary = 12000;
		break;
	default:
		cout << "销售经理的等级为1~3,请确认您的输入" << endl;
		break;
	}
}
