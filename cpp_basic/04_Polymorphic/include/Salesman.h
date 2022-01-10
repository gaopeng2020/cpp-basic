#pragma once
#include "Employee.h"
#include <iostream>
using namespace::std;

class Salesman :virtual public Employee
{
public:
	Salesman();
	virtual ~Salesman();
	virtual void initialization();
	virtual void monthSalary();
	virtual void changeLevel();
	double getSaleAmount();
protected:
	double saleAmount;
};

