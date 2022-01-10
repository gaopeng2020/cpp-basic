// 01_BasicKnowledge.cpp: 定义应用程序的入口点。
//

#include "01_BasicKnowledge.h"

using namespace std;

void cinCoutTest()
{
	int input = 0, sum = 0; 
	while (cin >> input)
	{
		sum += input;
		cout << "Sum is:" << sum << endl;
	}
}

void stringDefinition()
{
	//有=则是拷贝初始化，无等号则是直接初始化
	string s1;				//默认初始化， s1是一个空串
	string s2(s1);			//s2是s1的副本
	string s2_2("value");	//s3是字面值"value"的副本，除了字面值最后的那个空字符外
	string s3 = "value123"; //等价于s3 （"value"） ， s3是字面值"value"的副本
	string s4(10, 'c');		//把s4初始化为由连续n个字符c组成的串

	string s5 = s3 + s4 + "!!!";
	decltype(s3.size()) len3 = s3.size();
	auto len4 = s4.size();
	cout << "s3 size = " << len3 << "\ns4 size = " << len4 << "\ns5 size = " << s5.size() << endl;

	//范围for循环
	decltype(s5.size()) count_punt = 0;
	for (auto& char_r : s5)
	{
		if (ispunct(char_r)) //判断该字符是否是标点符号
			count_punt++;

		if (islower(char_r))		  //判断该字符是否是小写
			char_r = toupper(char_r); //通过引用将字符转换为大写，引用是变量的别名。
	}
	cout << count_punt << " punctuation char in " << s5 << endl;

	//通过下表索引字符串中的字符
	for (size_t i = 0; i < s5.size(); i++)
	{
		cout << s5[i];
	}
}

void getlineSemantic()
{
	string line;
	//getline（is,s）：从is(输入流)中读取一行赋给s，返回is
	while (getline(cin, line))
	{
		if (!line.empty())
			cout << line << " size = " << line.size() << endl;
	}
}

void vectorSemantic()
{
	//vector定义域初始化
	vector<int> v0;				 //v0为空的vector对象,常用
	vector<int> v1(10);			 //v1有10个元素，每个的值都是0
	vector<int> v2{ 10 };			 //v2有1个元素，该元素的值是10
	vector<int> v3(10, 1);		 //v3有10个元素，每个的值都是1
	vector<int> v4{ 10, 1 };		 //v4有2个元素，值分别是10和1
	vector<string> v5{ "hi" };	 //列表初始化：v5有一个元素
	vector<string> v6{ 10, "hi" }; //v6有10个值为"hi"的元素
	vector<string> v7{ 10 };		 //v7有10个默认初始化的元素

	//向vector对象中添加元素
	vector<string> text;
	string str;
	int n = 0;
	while (n < 3)
	{
		cout << "请输入字符串" << endl;
		cin >> str;
		text.push_back(str);
		n++;
	}
	//遍历获取vector元素
	cout << "方式一" << endl;
	for (auto i = 0; i < text.size(); i++)
	{
		cout << "text[" << i << "] is " << text[i] << endl;
	}
	//方式二：通过应用获取vector元素
	cout << "方式二" << endl;
	for (auto& r : text)
	{
		cout << r << endl;
	}
	//方式三：通过迭代器获取
	cout << "方式三" << endl;
	for (auto bgn = text.begin(), end = text.end(); bgn != end; bgn++)
	{
		cout << *bgn << endl;
	}
}
