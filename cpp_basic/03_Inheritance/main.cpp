#include "SmartPhone.h"
#include "FunctionalPhone.h"

void functionalPhoneTest(long long &phoneNum, const char *&message)
{
    std::cout << "================功能手机测试===============" << std::endl;
    //空参构造函数
    FunctionalPhone *fp1 = new FunctionalPhone();
    (*fp1).setPhoneNum(phoneNum);
    (*fp1).setMessage(message);
    printf("--------------调用空参构造函数-------------\n");
    printf("%lld\n%s\n", (*fp1).getPhoneNum(), (*fp1).getMessage());

    //全参数构造函数
    FunctionalPhone *fp2 = new FunctionalPhone();
    fp2 = fp1;
    printf("--------------调用全参构造函数--------------\n");
    (*fp1).callPhone();
    (*fp1).sendMessage();
}

void SmartPhoneTest(long long *phoneNum, const char **message, const char **webAddress)
{
    std::cout << "================智能手机测试===============" << std::endl;
    SmartPhone *sp1 = new SmartPhone(*phoneNum, *message, *webAddress);
    sp1->callPhone();
    sp1->sendMessage();
    sp1->surInternet();

    //SmartPhone sp2 = *sp1;
    SmartPhone sp2(*sp1);
    sp2.callPhone();
    sp2.sendMessage();
    sp2.surInternet();
    delete sp1;
}

void compatibilityTest(long long &phoneNum, const char *&message, const char *&webAddress)
{
    /*类型兼容性原则:
    1. 子类对象可以当作父类对象使用;
    2. 子类对象可以直接赋值给父类对象;
    3. 子类对象可以直接初始化父类对象;
    4. 父类指针可以直接指向子类对象;
    5. 父类引用可以直接引用子类对象;
    */
    std::cout << "==============类型兼容性测试===============" << std::endl;
    std::cout << "父类指针可以直接指向子类对象" << std::endl;
    FunctionalPhone *fp;
    SmartPhone *sp = new SmartPhone(phoneNum, message, webAddress);
    fp = sp;
    fp->callPhone();
    fp->sendMessage();

    std::cout << "父类引用可以直接引用子类对象" << std::endl;
    FunctionalPhone &funPhone = *sp;
    funPhone.callPhone();
    funPhone.sendMessage();
    delete sp;
}

int main(int argc, char const *argv[])
{
    long long phoneNum = 13025035768;
    const char *message = "test sending message";
    const char *webAddress = "https://www.google.com/";

    functionalPhoneTest(phoneNum, message);

    SmartPhoneTest(&phoneNum, &message, &webAddress);

    compatibilityTest(phoneNum, message, webAddress);
    return 0;
}
