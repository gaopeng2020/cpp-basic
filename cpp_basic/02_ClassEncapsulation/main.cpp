#include "VarString.h"

int main(int argc, char const *argv[])
{
    //new创建类对象需要指针接收，一处初始化，多处使用
    //new创建类对象使用完需delete销毁
    const char *str = "ABCDefghijklmn!@#$1234";
	VarString *var0 = new VarString();
	var0->setLen(strlen(str));
	var0->setStr(str);
    std::cout << "var0->getLen = " << var0->getLen() << std::endl;
    std::cout <<"var0->getStr = "<< var0->getStr() << std::endl;

	char array[20] = "ABC!@#12345678910";
	//VarString* var1 = new VarString(9, "ABC!@#123");
	VarString *var1 = new VarString(sizeof(array), array);

	//->前面放的是对象的指针，而.前面放的是对象
	 std::cout <<"var1->getLen = "<< var1->getLen() << std::endl;
	std::cout <<"var1->getStr = "<< var1->getStr() << std::endl;

	VarString var2("Abc!@#123");
	VarString var3 = var2;
	VarString var4(*var0);
	std::cout <<"var4->getLen = "<< var4.getLen() << std::endl;
    std::cout <<"var4->getStr = "<< var4.getStr() << std::endl;

    delete var0;
    delete var1;
    return 0;
}
