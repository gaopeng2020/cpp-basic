#include <ListTest.h>

void constructTest()
{
    //创建空的list容器，可以指定list容器的大小
    list<int> list1; //    list<int> list1(10);

    //列表初始化
    list<double> dlist{0.0f, 1.0f, 2.0f, 3.0f};

    //创建了一个包含 10 个元素并且值都为 255 个 list容器
    list<int> list2(10, 0xFF);

    //通过拷贝构造list
    list<int> list3(list2); //或者 list<int> list3 = list2;
    printTest("list3", list3);

    //拷贝普通数组，创建list容器
    int a[] = {1, 2, 3, 4, 5};
    list<int> list4(a, a + 5);
    printTest("list4", list4);

    //拷贝其它类型的容器，创建 list 容器
    array<int, 5> arr{11, 12, 13, 14, 15};
    list<int> list5(arr.begin() + 1, arr.end()); //拷贝arr容器中的{13,14,15}
    printTest("list5", list5);
}

list<int> addElementsTest()
{
    list<int> list1;
    list1.push_front(1);
    list1.push_back(2);
    list1.emplace_front(0);
    list1.emplace_back(3);
    list1.emplace(list1.end(), 4);

    /*  insert() 成员方法语法格式
语法格式	用法说明
iterator insert(pos,elem)	在迭代器 pos 指定的位置之前插入一个新元素 elem，并返回表示新插入元素位置的迭代器。
iterator insert(pos,n,elem)	在迭代器 pos 指定的位置之前插入 n 个元素 elem，并返回表示第一个新插入元素位置的迭代器。
iterator insert(pos,first,last) 在迭代器 pos 指定的位置之前，插入其他容器（例如 array、vector、deque 等）
中位于 [first,last) 区域的所有元素，并返回表示第一个新插入元素位置的迭代器。
iterator insert(pos,initlist)	在迭代器 pos 指定的位置之前，插入初始化列表（用大括号 { } 括起来的多个元素，
中间有逗号隔开）中所有的元素，并返回表示第一个新插入元素位置的迭代器。 */
    list1.insert(list1.end(), 5);
    list1.insert(list1.end(), 6, 6); //插入6个6

    array<int, 3> ary = {7, 8, 9};
    list1.insert(list1.end(), ary.begin(), ary.end());

    list1.insert(list1.end(), {10, 11, 12});

    /*     splice() 成员方法的作用对象是其它 list 容器，其功能是将其它 list 容器中的元素添加到当前 list 容器中指定位置处。
    splice() 成员方法的用法
语法格式	功能
void splice (iterator position, list& x);	position 为迭代器，用于指明插入位置；x 为另一个 list 容器。
此格式的 splice() 方法的功能是，将 x 容器中存储的所有元素全部移动当前 list 容器中 position 指明的位置处。

void splice (iterator position, list& x, iterator i);	position 为迭代器，用于指明插入位置；x 为另一个 list 容器；
i 也是一个迭代器，用于指向 x 容器中某个元素。此格式的 splice() 方法的功能是将 x 容器中 i 指向的元素移动到当前容器中 position 指明的位置处。

void splice (iterator position, list& x, iterator first, iterator last);	position 为迭代器，用于指明插入位置；
x 为另一个 list 容器；first 和 last 都是迭代器，[fist,last) 用于指定 x 容器中的某个区域。
此格式的 splice() 方法的功能是将 x 容器 [first, last) 范围内所有的元素移动到当前容器 position 指明的位置处。 */

    list<int> list2{13, 14, 15, 16, 17, 18, 19};
    list1.splice(list1.end(), list2);
    // list1.splice(list1.end(), list2, --list2.end());
    // list1.splice(list1.end(), list2, list2.begin(), list2.end());
    list1.emplace_back(19);
    list1.emplace_back(18);
    list1.emplace_back(10);
    printTest("list1", list1);

    // list 容器并不提供随机访问迭代器，只提供双向迭代器，因此不能对 list 中的元素使用 algorithm的sort() 算法。但是，list 模板定义了自己的 sort() 函数
    // sort(list1.begin(), list1.end(), greater<int>()); //不支持
    // list1.sort(less<>()); //升序
    // list1.sort(greater<>()); //降序
    list1.sort([](const int &l, const int &r)
               { return l > r; }); // Lambda自定义排序规则

    //Remove consecutive duplicate values from a sequence.
    auto _unique = unique(list1.begin(), list1.end());
    list1.erase(_unique, list1.end());

    list1.pop_back();
    printTest("list1", list1);
    return list1;
}

void iteratorTest()
{
    /*     list 配备的迭代器类型为双向迭代器，而不再是随机访问迭代器。
这意味着，假设 p1 和 p2 都是双向迭代器，则它们支持使用 ++p1、 p1++、 p1--、 --p1、 *p1、 p1==p2 以及 p1!=p2 运算符，
但不支持以下操作（其中 i 为整数）：
不能通过下标[]访问 list 容器中指定位置处的元素。
双向迭代器 p1 不支持使用 -=、+=、+、- 运算符。
双向迭代器 p1、p2 不支持使用 <、 >、 <=、 >= 比较运算符。 */
    list<char> values{'h', 't', 't', 'p', ':', '/', '/', 'c', '.', 'b', 'i', 'a', 'n', 'c', 'h', 'e', 'n', 'g', '.', 'n', 'e', 't'};
    //使用begin()/end()迭代器函数对输出list容器中的元素
    for (list<char>::iterator it = values.begin(); it != values.end(); ++it)
    {
        cout << *it;
    }
    cout << endl;
    //使用 rbegin()/rend()迭代器函数输出 lsit 容器中的元素
    for (list<char>::reverse_iterator it = values.rbegin(); it != values.rend(); ++it)
    {
        cout << *it;
    }

    /*     list 容器在进行插入（insert()）、接合（splice()）等操作时，都不会造成原有的 list 迭代器失效，
    甚至进行删除操作，而只有指向被删除元素的迭代器失效，其他迭代器不受任何影响 */
}

void accessDeleteTest()
{
    /*     list 容器不支持随机访问，未提供下标操作符 [] 和 at() 成员函数，也没有提供 data() 成员函数。
通过 front() 和 back() 成员函数，可以分别获得 list 容器中第一个元素和最后一个元素的引用形式;
除此之外，如果想访问 list 容存储的其他元素，就只能使用 list 容器的迭代器 */
    list<int> list1 = addElementsTest();
    cout << "list1.front() = " << list1.front() << " list1.back() = " << list1.back() << endl;

    for (auto it = list1.begin(); it != list1.end(); ++it)
    {
        if (*it == 10)
        {
            cout << "值为10的元素找到了" << endl;
        }
    }
}