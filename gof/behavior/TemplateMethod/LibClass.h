#pragma once
#include <iostream>
#include <string>
using namespace std;
class LibClass
{
public:
	virtual void step1() = 0;
	void step2();
	virtual int step3() = 0;
	void step4();

	void run();
};

