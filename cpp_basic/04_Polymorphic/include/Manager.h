#pragma once
#include "Employee.h"
#include <iostream>
using namespace::std;

class Manager : virtual public Employee
{
public:
	Manager();
	~Manager() override;
	void initialization() override;
	void monthSalary() override;
	void changeLevel() override;
	virtual void getBaseSalary();

protected:
	int baseSalary;
};

