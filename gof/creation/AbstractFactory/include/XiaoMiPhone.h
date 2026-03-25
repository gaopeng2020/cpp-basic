#pragma once
#include "IPhone.h"
class XiaoMiPhone : public IPhone
{
public:
	virtual void phoneInfo() override;
	~XiaoMiPhone();
	string getPoneName();

private:
	string phoneName;
};

