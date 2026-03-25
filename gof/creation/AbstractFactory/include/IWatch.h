#pragma once
#include <string>
#include <iostream>
using namespace std;
class IWatch
{
public:
	virtual void watchInfo() = 0;
	virtual ~IWatch() {};
};

