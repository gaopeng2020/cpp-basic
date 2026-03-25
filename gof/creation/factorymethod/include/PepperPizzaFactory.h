#pragma once
#include "IPizzaFactory.h"
#include "PepperPizza.h"
class PepperPizzaFactory : public IPizzaFactory
{
public:
	virtual IPizza* createPizza() override;
	~PepperPizzaFactory();
};

