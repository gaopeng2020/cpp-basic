#include "VarString.h"

	//无参构造
	VarString::VarString()
	{
		this->len = 0;
		this->str = NULL;
	}

	//全参构造函数
	VarString::VarString(int len, const char *str)
	{
		this->len = len;
		this->str = (char *)malloc(len + 1);
		//strcpy必须引入<cstring>
		strcpy(this->str, str);
	}

	// copy构造函数作用：用一个对象初始化另外一个对象,系统提供的默认拷贝构造器是通常意义上的浅拷贝。
	//当数据成员中有指针时，使用浅拷贝，则两对象中的两个指针将指向同一个地址，
	//当对象快结束时，会调用两次析构函数，而导致指针悬挂现象，造成内存泄露或程序崩溃。
	//简而言之，当数据成员中有指针时，必须要用深拷贝.

	//深拷贝构造函数，适用于VarString var2 = var2
	VarString::VarString(const VarString &varStr)
	{
		this->len = varStr.len;
		this->str = (char *)malloc(len + 1);
		strcpy(this->str, varStr.str);
	}

	//深拷贝构造函数，适用于VarString var2 (var2)
	VarString::VarString(const char *varStr)
	{
		this->len = strlen(varStr);
		this->str = (char *)malloc(len + 1);
		strcpy(this->str, varStr);
	}

	//析构函数:析构函数的作用,并不是删除对象,而在对象销毁前完成的一些清理工作
	VarString::~VarString()
	{
		if (str != NULL)
		{
			free(str);
			len = 0;
			str = NULL;
		}
	}

	int VarString::getLen()
	{
		return len;
	}
	void VarString::setLen(int len)
	{
		this->len = len;
	}

	char *VarString::getStr()
	{
		return str;
	}
	void VarString::setStr(const char *str)
	{
		if (getLen() == 0)
		{
			this->str = (char *)malloc(strlen(str) + 1);
		}
		else
		{
			this->str = (char *)malloc(getLen() + 1);
		}
		strcpy(this->str, str);
	}
