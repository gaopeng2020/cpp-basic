#pragma once
#include "IComputer.h"
class AppleComputer : public IComputer
{
public:
	virtual void computerInfo() override;
	virtual ~AppleComputer();
};

