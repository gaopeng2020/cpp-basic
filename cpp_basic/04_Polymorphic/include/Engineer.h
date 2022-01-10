#pragma once
#include "Employee.h"
#include <iostream>
using namespace::std;

class Engineer : public Employee
{
public:
	Engineer();
	virtual ~Engineer();
	virtual void initialization();
	virtual void monthSalary();
	virtual void changeLevel();
	float getovertimeHours();
protected:
	float overtimeHour;
};

