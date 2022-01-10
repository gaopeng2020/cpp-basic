#include "Employee.h"

int Employee::id = 1;
Employee::Employee()
{
	name = "";
	id++;
	level = 1;
	salary = 0.0;
}
Employee::~Employee()
{
}

void Employee::employeeInfo()
{
	cout << "姓名\t"
		 << "ID\t"
		 << "岗位等级\t"
		 << "月薪\t" << endl;
	cout << name << "\t" << id << "\t" << level << "\t" << salary << "\t" << endl;
}
