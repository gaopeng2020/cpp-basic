#include "FunctionalPhone.h"
//构造函数
FunctionalPhone::FunctionalPhone()
{
    this->phoneNum = 0;
    this->message = nullptr;
}
FunctionalPhone::FunctionalPhone(long long phoneNum, const char *message):phoneNum(phoneNum),message(message)
{
//    this->phoneNum = phoneNum;
//    this->message = message;
}
FunctionalPhone::FunctionalPhone(const FunctionalPhone &funcPhone):phoneNum(funcPhone.phoneNum),message(funcPhone.message)
{
//    this->phoneNum = funcPhone.phoneNum;
//    this->message = funcPhone.message;
}

//功能函数
void FunctionalPhone::callPhone() const
{
    std::cout << "拨打电话： " << phoneNum << std::endl;
}
void FunctionalPhone::sendMessage()
{
    std::cout << "发送短信：" << message << std::endl;
}

//析构函数
FunctionalPhone::~FunctionalPhone()
{
    //if (message!=NULL)
    //{
    //    free(message);
    //    this->phoneNum = 0;
    //    this->message = NULL;
    //}
}

long long FunctionalPhone::getPhoneNum() const
{
    return this->phoneNum;
}
void FunctionalPhone::setPhoneNum(long long phoneNum)
{
    this->phoneNum = phoneNum;
}
const char *FunctionalPhone::getMessage()
{
    return this->message;
}
void FunctionalPhone::setMessage(const char *message)
{
    this->message = message;
}

