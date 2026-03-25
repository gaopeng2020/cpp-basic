#pragma once
#include "IPizza.h"
class IPizzaFactory
{
public:
	virtual IPizza* createPizza() = 0;
	~IPizzaFactory() {};
};
