#pragma once
#include "IProductFactory.h"
#include "ApplePhone.h"
#include "AppleComputer.h"
#include "AppleWatch.h"
class AppleProductFactory : public IProductFactory
{
	virtual IPhone* createPhone() override;
	virtual IComputer* createComputer() override;
	virtual IWatch* createWatche() override;
};

