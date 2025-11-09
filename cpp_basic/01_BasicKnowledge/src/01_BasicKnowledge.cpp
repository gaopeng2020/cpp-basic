// 01_BasicKnowledge.cpp: 定义应用程序的入口点。
//

#include "01_BasicKnowledge.h"

using namespace std;

void cinCoutTest() {
    int input = 0, sum = 0;
    while (cin >> input) {
        sum += input;
        cout << "Sum is:" << sum << endl;
    }
}

void stringDefinition() {
    //有=则是拷贝初始化，无等号则是直接初始化
    string s1; //默认初始化， s1是一个空串
    string s2(s1); //s2是s1的副本
    string s2_2("value"); //s3是字面值"value"的副本，除了字面值最后的那个空字符外
    string s3 = "value123"; //等价于s3 （"value"） ， s3是字面值"value"的副本
    string s4(10, 'c'); //把s4初始化为由连续n个字符c组成的串

    string s5 = s3 + s4 + "!!!";
    decltype(s3.size()) len3 = s3.size();
    auto len4 = s4.size();
    cout << "s3 size = " << len3 << "\ns4 size = " << len4 << "\ns5 size = " << s5.size() << endl;

    //范围for循环
    decltype(s5.size()) count_punt = 0;
    for (auto &char_r: s5) {
        if (ispunct(char_r)) //判断该字符是否是标点符号
            count_punt++;

        if (islower(char_r)) //判断该字符是否是小写
            char_r = toupper(char_r); //通过引用将字符转换为大写，引用是变量的别名。
    }
    cout << count_punt << " punctuation char in " << s5 << endl;

    //通过下表索引字符串中的字符
    for (size_t i = 0; i < s5.size(); i++) {
        cout << s5[i];
    }
}

void getlineSemantic() {
    string line;
    //getline（is,s）：从is(输入流)中读取一行赋给s，返回is
    while (getline(cin, line)) {
        if (!line.empty())
            cout << line << " size = " << line.size() << endl;
    }
}

void vectorSemantic() {
    //vector定义域初始化
    vector<int> v0; //v0为空的vector对象,常用
    vector<int> v1(10); //v1有10个元素，每个的值都是0
    vector<int> v2{10}; //v2有1个元素，该元素的值是10
    vector<int> v3(10, 1); //v3有10个元素，每个的值都是1
    vector<int> v4{10, 1}; //v4有2个元素，值分别是10和1
    vector<string> v5{"hi"}; //列表初始化：v5有一个元素
    vector<string> v6{10, "hi"}; //v6有10个值为"hi"的元素
    vector<string> v7{10}; //v7有10个默认初始化的元素

    //向vector对象中添加元素
    vector<string> text;
    string str;
    int n = 0;
    while (n < 3) {
        cout << "请输入字符串" << endl;
        cin >> str;
        text.push_back(str);
        n++;
    }
    //遍历获取vector元素
    cout << "方式一" << endl;
    for (size_t i = 0; i < std::size(text); i++) {
        cout << "text[" << i << "] is " << text[i] << endl;
    }
    //方式二：通过应用获取vector元素
    cout << "方式二" << endl;
    for (auto &item: text) {
        cout << item << endl;
    }
    //方式三：通过迭代器获取
    cout << "方式三" << endl;
    for (auto iterator = text.begin(), end = text.end(); iterator != end; iterator++) {
        cout << *iterator << endl;
    }
}

/*
* `std::shared_ptr` 是一种智能指针，它能够记录多少个 `shared_ptr` 共同指向一个对象，从而消除显式的调用`delete`，
* 当引用计数变为零的时候就会将对象自动删除。
* `std::make_shared` 就能够用来消除显式的使用 `new`，所以`std::make_shared` 会分配创建传入参数中的对象，
* 并返回这个对象类型的`std::shared_ptr`指针
* `std::shared_ptr` 可以通过 `get()` 方法来获取原始指针，通过 `reset()` 来减少一个引用计数，
* 并通过`use_count()`来查看一个对象的引用计数
 */
void shared_ptr_test() {
    auto pointer = std::make_shared<int>(10);
    cout << pointer.get() << endl;
    auto pointer2 = pointer; // 引用计数+1
    auto pointer3 = pointer; // 引用计数+1
    int *p = pointer.get(); // 这样不会增加引用计数
    std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl; // 3
    std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl; // 3
    std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl; // 3

    pointer2.reset();
    std::cout << "reset pointer2:" << std::endl;
    std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl; // 2
    std::cout << "pointer2.use_count() = "
            << pointer2.use_count() << std::endl; // pointer2 已 reset; 0
    std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl; // 2

    pointer3.reset();
    std::cout << "reset pointer3:" << std::endl;
    std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl; // 1
    std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl; // 0
    std::cout << "pointer3.use_count() = "
            << pointer3.use_count() << std::endl; // pointer3 已 reset; 0
}
/*
 * `std::unique_ptr` 是一种独占的智能指针，它禁止其他智能指针与其共享同一个对象，从而保证代码的安全
 * 可以利用 `std::move` 将其转移给其他的 `unique_ptr`
 * 如果A引用B，B也引用A，此时如果使用std::shared_ptr则会导致A和B最终都无法释放，此时需要std::weak_pt，、
 * `std::weak_ptr`是一种弱引用，弱引用不会引起引用计数增加
 */
void unique_ptr_test() {
    std::unique_ptr<int> p1 = std::make_unique<int>(10); // make_unique 从 C++14 引入
    // std::unique_ptr<int> pointer2 = pointer; // 非法
    *p1 = 15;
    cout<<"p1 = "<< *p1<<endl;
    auto p2 = std::move(p1);
    cout<<"p2 = "<< *p2<<endl;
}
