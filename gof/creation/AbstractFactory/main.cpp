#include "AbstractFactoryClientApp.h"
#include "XiaoMIProductFactory.h"
#include "AppleProductFactory.h"

int main() {
	AppleProductFactory apple;
	AbstractFactoryClientApp appleProduct(apple);
	IPhone* absTractPhone = appleProduct.getPhone();
	absTractPhone->phoneInfo();
	appleProduct.getComputer()->computerInfo();
	appleProduct.getWatch()->watchInfo();



	XiaoMIProductFactory xiaoMi;
	AbstractFactoryClientApp xiaoMIProduct(xiaoMi);
	IPhone* phone = xiaoMIProduct.getPhone();
	if (typeid(*phone) == typeid(XiaoMiPhone))
	{
		XiaoMiPhone* miPhone = dynamic_cast<XiaoMiPhone*>(phone);
		miPhone->phoneInfo();
		cout << "PoneName = " << miPhone->getPoneName() << endl;
	}

	xiaoMIProduct.getComputer()->computerInfo();
	xiaoMIProduct.getWatch()->watchInfo();
}