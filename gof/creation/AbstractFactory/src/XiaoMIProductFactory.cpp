#include "XiaoMIProductFactory.h"
IPhone* XiaoMIProductFactory::createPhone() {
	return new XiaoMiPhone();
}
IComputer* XiaoMIProductFactory::createComputer() {
	return new XiaoMiComputer();
}
IWatch* XiaoMIProductFactory::createWatche() {
	return new XiaoMiWatch();
}
