#pragma once
#include "IPhone.h"
#include "IComputer.h"
#include "IWatch.h"
class IProductFactory
{
public:
	virtual IPhone* createPhone() = 0;
	virtual IComputer* createComputer() = 0;
	virtual IWatch* createWatche() = 0;

	virtual ~IProductFactory() {};
};

