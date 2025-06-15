#pragma once
#include "Employee.h"
#include <iostream>
using namespace::std;

class Salesman :virtual public Employee
{
public:
	Salesman();
	~Salesman() override;
	void initialization() override;
	void monthSalary() override;
	void changeLevel() override;

    [[maybe_unused]] double getSaleAmount() const;
protected:
	double saleAmount;
};

