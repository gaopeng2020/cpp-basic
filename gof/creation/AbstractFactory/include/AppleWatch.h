#pragma once
#include "IWatch.h"
class AppleWatch : public IWatch
{
public:
	virtual void watchInfo() override;
	~AppleWatch();
};

