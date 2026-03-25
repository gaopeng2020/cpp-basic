#pragma once
#include "IPizza.h"
#include <iostream>
#include <string>
using namespace std;
class PepperPizza : public IPizza
{
public:
public:
	virtual void prepare() override;
	virtual void bake() override;
	virtual void cut() override;
	virtual void box() override;
};

