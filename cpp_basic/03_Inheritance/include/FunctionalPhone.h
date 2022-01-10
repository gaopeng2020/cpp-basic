#pragma once
#include <iostream>

class FunctionalPhone
{
private:
    long long phoneNum;
    const char *message;

public:
    //构造函数
    FunctionalPhone();
    FunctionalPhone(long long phoneNum, const char *message);
    FunctionalPhone(const FunctionalPhone &funcPhone);

    //功能函数
    void callPhone();
    void sendMessage();

    //析构函数
    ~FunctionalPhone();

    long long getPhoneNum();
    void setPhoneNum(long long phoneNum);
    const char *getMessage();
    void setMessage(const char *message);
};
