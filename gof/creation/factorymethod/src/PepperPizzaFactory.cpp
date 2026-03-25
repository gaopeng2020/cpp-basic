#include "PepperPizzaFactory.h"

IPizza* PepperPizzaFactory::createPizza()
{
	return new PepperPizza();
}

PepperPizzaFactory::~PepperPizzaFactory()
{
}
