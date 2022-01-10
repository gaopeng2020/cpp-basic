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
	~SalseManager();
	virtual void initialization();
	virtual void monthSalary();
	virtual void changeLevel();
	virtual void getBaseSalary();
};

