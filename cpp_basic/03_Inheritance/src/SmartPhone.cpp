#include "SmartPhone.h"

//phoneNum与message用父类去构造，webAddress子类自己去构造；
SmartPhone::SmartPhone() : FunctionalPhone()
{
    this->webAddress = nullptr;
}
SmartPhone::SmartPhone(long long phoneNum, const char *message, const char *webAddress) : FunctionalPhone(phoneNum, message)
{
    this->webAddress = webAddress;
}
SmartPhone::SmartPhone(const SmartPhone &smartPhone) : FunctionalPhone(smartPhone)
{
    this->webAddress = smartPhone.webAddress;
}

SmartPhone::~SmartPhone()
{
    if (this->webAddress != nullptr)
    {
        this->webAddress = nullptr;
    }
}

//Smart Phone new Function
void SmartPhone::surInternet()
{
    std::cout << "浏览网页：" << webAddress << std::endl;
}

const char *SmartPhone::getWebAddress()
{
    return this->webAddress;
}
void SmartPhone::setwebAddress(const char *webAddress)
{
    this->webAddress = webAddress;
}

