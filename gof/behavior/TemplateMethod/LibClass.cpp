#include "LibClass.h"

void LibClass::step2()
{
	cout << "step2(LibClass) executed succesfully!" << endl;
}

void LibClass::step4()
{
	cout << "step4(LibClass) executed succesfully! " << endl;
}

void LibClass::run()
{
	step1();
	for (size_t i = 0; i < 3; i++)
	{
		step2();
	}
	int step3Result = step3();
	if (step3Result >= 0)
	{
		step4();
	}
}
