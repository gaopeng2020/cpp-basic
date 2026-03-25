#include "CheesePizzaFactory.h"


IPizza* CheesePizzaFactory::createPizza()
{
	IPizza* pizza = new CheesePizza();
	return pizza;
}

CheesePizzaFactory::~CheesePizzaFactory()
{
}
