#pragma once
#include "IPizza.h"
#include <iostream>
#include <string>
using namespace std;
class CheesePizza : public IPizza
{
public:
	virtual void prepare() override;
	virtual void bake() override;
	virtual void cut() override;
	virtual void box() override;
};

