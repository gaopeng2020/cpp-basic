#define _CTR_SECURE_NO_WARNINGS
//如果模板类的声明与实现分开了，则调用者需要包含实现文件，即cpp文件，此时可以通过将木板编译成库文件

//推荐的做法，将模板类的声明与实现放在一个hpp文件中完成
#include "OperatorOverride.hpp"
using namespace std;
int main()
{
	OperatorOverride<int> oprOrd0(1, 1); //全参构造函数

	string oprOrd1Name = oprOrd0(5, 5); //仿函数：把类对象像函数名一样使用,在构造时就实现某个功能并将结果返回
	cout << "oprOrd1Name = " << oprOrd1Name << endl;
	oprOrd0.toString();

	OperatorOverride<double> oprOrd1(1.0, 1.0); //全参构造函数

	OperatorOverride<double> oprOrd2(1.0, 1.0);

	OperatorOverride<double> oprOrd3 = oprOrd1 + oprOrd2;
	//oprOrd3.toString();

	OperatorOverride<double> oprOrd4 = ++++oprOrd2;
	oprOrd4 = oprOrd2++;
	oprOrd2.toString();
	oprOrd4 += oprOrd3;
	oprOrd4.toString();

	OperatorOverride<double> oprOrd5 = oprOrd4 / oprOrd2;
	oprOrd5.toString();
}