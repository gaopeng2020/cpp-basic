#pragma once
#include "Employee.h"
#include <iostream>
using namespace::std;

class Engineer : public Employee
{
public:
	Engineer();
	~Engineer() override;
	void initialization() override;
	void monthSalary() override;
	void changeLevel() override;
	[[nodiscard]] float getOvertimeHours() const;
protected:
	float overtimeHour;
};

