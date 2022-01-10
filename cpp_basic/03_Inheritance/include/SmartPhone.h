#pragma once
#include "FunctionalPhone.h"
/*
继承:是新的类从已有类那里得到已有的特性。
派生:从已有类产生新类的过程就是类的派生。
原有的类称为基类或父类,产生的新类称为派生类或子类。
派生与继承,是同一种意义两种称谓。  isA 的关系

继承权限：
1、需要被外界访问的成员直接设置为public;
2、只能在当前类中访问的成员设置为private;
3、只能在当前类和子类中访问的成员设置为protected;
   通过公有继承，派生类得到了基类中除构造函数、析构函数之外的所有成员.

类型兼容性原则:
1. 子类对象可以当作父类对象使用;
2. 子类对象可以直接赋值给父类对象;
3. 子类对象可以直接初始化父类对象;
4. 父类指针可以直接指向子类对象;
5. 父类引用可以直接引用子类对象;
子类就是特殊的父类 (base *p = &child;)

*/
class SmartPhone : public FunctionalPhone
{
private:
    const char *webAddress;

public:
    //phoneNum与message用父类去构造，webAddress子类自己去构造；
    SmartPhone();
    SmartPhone(long long phoneNum, const char *message, const char *webAddress);
    SmartPhone(const SmartPhone &smartPhone);

    ~SmartPhone();

    //Smart Phone new Function
    void surInternet();

    const char *getWebAddress();
    void setwebAddress(const char *webAddress);
};

