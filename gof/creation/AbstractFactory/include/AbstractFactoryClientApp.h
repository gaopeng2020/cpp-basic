#pragma once
#include "IPhone.h"
#include "IComputer.h"
#include "IWatch.h"
#include "IProductFactory.h"
class AbstractFactoryClientApp
{
public:
	AbstractFactoryClientApp(IProductFactory& productFactory);
	~AbstractFactoryClientApp();

	IPhone* getPhone();
	IComputer* getComputer();
	IWatch* getWatch();

private:
	IPhone* phone;
	IComputer* computer;
	IWatch* watch;
	IProductFactory& factory;
	void createProduct();
};

