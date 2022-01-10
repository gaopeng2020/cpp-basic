#include "Engineer.h"

Engineer::Engineer() : Employee()
{
	this->overtimeHour = 0.0;
}
Engineer::~Engineer()
{
}
void Engineer::initialization()
{
	cout << "请输入工程师姓名" << endl;
	cin >> name;
	cout << "请输入工程师岗位等级" << endl;
	cin >> level;
	if (level > 6 || level < 0)
	{
		cout << "工程师的等级为1~6,请确认您的输入" << endl;
		cin >> level;
	}
}
void Engineer::monthSalary()
{
	if (overtimeHour == 0.0)
	{
		cout << "请输入员工加班时长" << endl;
		cin >> overtimeHour;
	}

	int baseSalary = 0;
	switch (level)
	{
	case 1:
		baseSalary = 5000;
		salary = baseSalary * (1 + overtimeHour * (1 / 22.0 / 8.0));
		break;
	case 2:
		baseSalary = 8000;
		salary = baseSalary * (1 + overtimeHour * (1 / 22.0 / 8.0));
		break;
	case 3:
		baseSalary = 12000;
		salary = baseSalary * (1 + overtimeHour * (1 / 22.0 / 8.0));
		break;
	case 4:
		baseSalary = 15000;
		salary = baseSalary * (1 + overtimeHour * (1 / 22.0 / 8.0));
		break;
	case 5:
		baseSalary = 18000;
		salary = baseSalary * (1 + overtimeHour * (1 / 22.0 / 8.0));
		break;
	case 6:
		baseSalary = 20000;
		salary = baseSalary * (1 + overtimeHour * (1 / 22.0 / 8.0));
		break;
	default:
		cout << "工程师的等级为1~6,请确认您的输入" << endl;
		break;
	}
	cout << name << ":本月总工资为：" << salary << endl;
}
void Engineer::changeLevel()
{
	cout << "请输入 " << name << " 新的岗位等级" << endl;
	cin >> level;
	if (level > 6 || level < 0)
	{
		cout << "工程师的等级为1~6,请确认您的输入" << endl;
		cin >> level;
	}
	Engineer::monthSalary();
	cout << name << ":新的岗位等级为" << level << endl;
}
float Engineer::getovertimeHours()
{
	return overtimeHour;
}