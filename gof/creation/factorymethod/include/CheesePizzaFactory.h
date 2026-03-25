#pragma once
#include "IPizzaFactory.h"
#include "CheesePizza.h"
class CheesePizzaFactory : public IPizzaFactory
{
public:
	virtual IPizza* createPizza() override;
	~CheesePizzaFactory();
};

