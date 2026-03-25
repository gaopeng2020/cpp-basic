#include "FactoryMethodApp.h"

FactoryMethodApp::FactoryMethodApp()
{
	this->pizza = nullptr;
}

FactoryMethodApp::FactoryMethodApp(IPizzaFactory& pizzaFactory)
{
	createPizzaByFactory(pizzaFactory);
}

FactoryMethodApp::~FactoryMethodApp()
{
	if (pizza != nullptr)
	{
		delete pizza;
		pizza = nullptr;
	}
}

IPizza* FactoryMethodApp::getPizza()
{
	return this->pizza;
}

IPizza* FactoryMethodApp::createPizzaByFactory(IPizzaFactory& pizzaFactory) {
	this->pizza = pizzaFactory.createPizza();
	this->pizza->prepare();
	this->pizza->bake();
	this->pizza->cut();
	this->pizza->box();
	return this->pizza;
}