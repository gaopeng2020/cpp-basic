#pragma once
#include "IComputer.h"
class XiaoMiComputer : public IComputer
{
public:
	virtual void computerInfo() override;
	virtual ~XiaoMiComputer();
};

