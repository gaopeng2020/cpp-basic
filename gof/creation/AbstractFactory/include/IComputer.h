#pragma once
#include <string>
#include <iostream>
using namespace std;
class IComputer
{
public:
	virtual void computerInfo() = 0;
	virtual ~IComputer() {};
};

