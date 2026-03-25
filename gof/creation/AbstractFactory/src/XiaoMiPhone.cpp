#include "XiaoMiPhone.h"

void XiaoMiPhone::phoneInfo()
{
	string info = "This is XiaoMi Phone";
	this->phoneName = "xiaomi11";
	cout << info << endl;
}

XiaoMiPhone::~XiaoMiPhone()
{
}

string XiaoMiPhone::getPoneName()
{
	return this->phoneName;
}
