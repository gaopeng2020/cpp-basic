#pragma once
#include <string>
#include <iostream>
using namespace::std;

class Employee
{public:
	Employee();
	virtual ~Employee();
	virtual void initialization() = 0;
	virtual void monthSalary() = 0;
	virtual void changeLevel() = 0;
	void employeeInfo();

protected:
	string name;
	static int id;
	int level;
	double salary;
};

