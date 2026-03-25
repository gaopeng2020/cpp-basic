#include "AbstractFactoryClientApp.h"

AbstractFactoryClientApp::AbstractFactoryClientApp(IProductFactory& productFactory) :factory(productFactory)
{
	createProduct();
}

AbstractFactoryClientApp::~AbstractFactoryClientApp()
{
	delete phone;
	delete computer;
	delete watch;
}

void AbstractFactoryClientApp::createProduct()
{
	this->phone = factory.createPhone();
	this->computer = factory.createComputer();
	this->watch = factory.createWatche();
}

IPhone* AbstractFactoryClientApp::getPhone()
{
	return this->phone;
}

IComputer* AbstractFactoryClientApp::getComputer()
{
	return this->computer;
}

IWatch* AbstractFactoryClientApp::getWatch()
{
	return this->watch;
}
