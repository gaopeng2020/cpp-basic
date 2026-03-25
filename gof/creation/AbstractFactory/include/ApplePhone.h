#pragma once
#include "IPhone.h"
class ApplePhone : public IPhone
{
public:
	virtual void phoneInfo() override;
	~ApplePhone();
};

