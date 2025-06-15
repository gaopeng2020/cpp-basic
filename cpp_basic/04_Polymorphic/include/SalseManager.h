#pragma once
#include "Employee.h"
#include "Salesman.h"
#include "Manager.h"
#include <iostream>
using namespace::std;

class SalseManager :public Salesman,public Manager
{
public:
	SalseManager();
	~SalseManager() override;
	void initialization() override;
	void monthSalary() override;
	void changeLevel() override;
	void getBaseSalary() override;
};

