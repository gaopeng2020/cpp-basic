#include "AppleProductFactory.h"
IPhone* AppleProductFactory::createPhone() {
	return new ApplePhone();
}
IComputer* AppleProductFactory::createComputer() {
	return new AppleComputer();
}
IWatch* AppleProductFactory::createWatche() {
	return new AppleWatch();
}