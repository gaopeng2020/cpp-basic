#pragma once
#include "Employee.h"
#include <iostream>
using namespace::std;

class Manager : virtual public Employee
{
public:
	Manager();
	virtual ~Manager();
	virtual void initialization();
	virtual void monthSalary();
	virtual void changeLevel();
	virtual void getBaseSalary();

protected:
	int baseSalary;
};

