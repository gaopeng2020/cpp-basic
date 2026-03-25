#pragma once
#include "IProductFactory.h"
#include "XiaoMiPhone.h"
#include "XiaoMiComputer.h"
#include "XiaoMiWatch.h"
class XiaoMIProductFactory : public IProductFactory
{
public:
	virtual IPhone* createPhone() override;
	virtual IComputer* createComputer() override;
	virtual IWatch* createWatche() override;

};

