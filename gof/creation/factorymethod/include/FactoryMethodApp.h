#pragma once
#include "IPizza.h"
#include "IPizzaFactory.h"

class FactoryMethodApp
{
public:
	FactoryMethodApp();
	FactoryMethodApp(IPizzaFactory& pizzaFactory);

	~FactoryMethodApp();

	IPizza* getPizza();

	IPizza* createPizzaByFactory(IPizzaFactory& pizzaFactory);

private:
	IPizza* pizza;
};

