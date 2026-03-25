#pragma once
#include "IWatch.h"
class XiaoMiWatch : public IWatch
{
public:
	virtual void watchInfo() override;
	~XiaoMiWatch();
};

