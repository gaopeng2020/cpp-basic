#include "LibClass.h"
#include "AppClass.h"

int main() {
	LibClass* lib = new AppClass();
	lib->run();
}
