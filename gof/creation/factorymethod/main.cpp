#include "FactoryMethodApp.h"
#include "CheesePizzaFactory.h"
#include "PepperPizzaFactory.h"

int main()
{
	FactoryMethodApp app;

	//chesse pizza
	CheesePizzaFactory cheesePizzaFactory;
	app.createPizzaByFactory(cheesePizzaFactory);

	//PepperPizza
	PepperPizzaFactory pepperPizzaFactory;
	app.createPizzaByFactory(pepperPizzaFactory);

}