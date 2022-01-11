#include "DequeTest.h"
void constructTest()
{
    //1. 创建一个没有任何元素的空 deque 容器：
    deque<int> deq1;

    //2. 创建一个具有 n 个元素的 deque 容器，其中每个元素都采用对应类型的默认值：
    deque<float> deq2(20);

    //3. 创建一个具有 n 个元素的 deque 容器，并为每个元素都指定初始值
    deque<double> deq3(30, 0.0L);

    //3. 拷贝构造或者拷贝赋值运算符
    deque<double> deq4(deq3);
    deque<double> deq5 = deq4;
    printTest("deq5", deq5);

    //4.通过其他容器构造
    array<int, 6> arr{11, 12, 13, 14, 15, 16};
    list<int> lst(arr.begin(), arr.end());
    deque<int> deq6(lst.begin(), lst.end());

    printTest("deq6", deq6);
}

deque<int> addElementsTest()
{
    deque<int> deq;
    deq.assign(5, 2);
    deq.push_back(1);
    deq.push_front(-1);
    deq.push_front(-2);
    deq.emplace(deq.end(), 3);
    deq.emplace_back(4);
    deq.emplace_front(0);

    /*    
    iterator insert(pos,elem)	在迭代器 pos 指定的位置之前插入一个新元素elem，并返回表示新插入元素位置的迭代器。
    iterator insert(pos,n,elem)	在迭代器 pos 指定的位置之前插入 n 个元素 elem，并返回表示第一个新插入元素位置的迭代器。
    iterator insert(pos,first,last) 	在迭代器 pos 指定的位置之前，插入其他容器（不仅限于vector）中位于 [first,last) 区域的所有元素，并返回表示第一个新插入元素位置的迭代器。
    iterator insert(pos,initlist)	在迭代器 pos 指定的位置之前，插入初始化列表（用大括号{}括起来的多个元素，中间有逗号隔开）中所有的元素，并返回表示第一个新插入元素位置的迭代器。 */
    deq.insert(deq.begin() + 1, 3); //{1,3,2}
    deq.insert(deq.end(), 3, 10);

    list<int> lst{5, 6, 7, 8};
    deq.insert(deq.end(), lst.begin(), lst.end());
    deq.insert(deq.end(), {15, 16, 17, 18, 19, 20, 20, 20});

    array<int, 5> arry{11, 12, 13, 14, 15};
    for (size_t i = 0; i < arry.size(); i++)
    {
        deq.emplace_back(arry.at(i));
    }

    //assign会覆盖原有的元素
    // deq.assign(arry.begin(), arry.end());

    return deq;
}

void iteratorTest(deque<int> &deq)
{
    cout << "=======deq iterator test start=======" << endl;
    for (deque<int>::iterator it = deq.begin(); it != deq.end(); ++it)
    {
        cout << *it << endl;
    }
    cout << "========deq iterator test end========" << endl;

    cout << "========deq reverse_iterator test end========" << endl;
    for (deque<int>::reverse_iterator it = deq.rend() + 1; it >= deq.rbegin(); --it)
    {
        cout << *it << endl;
    }
    cout << "=======deq reverse_iterator test end=======" << endl;
}

void accessDeleteTest(deque<int> &deq)
{
    deq.pop_front();
    deq.pop_back();
    // sort(deq.begin(), deq.end(), less<int>());
    sort(deq.begin(), deq.end(), [](const int &l, const int &r)
         { return l < r; });
    deque<int>::iterator uniq = unique(deq.begin(), deq.end());
    deq.erase(uniq, deq.end());

    cout << "deq.at(5) = " << deq.at(5) << endl;
    cout << "deq[10] = " << deq[10] << endl;
    cout << "deq.front() = " << deq.front() << endl;
    cout << "deq.back() = " << deq.back() << endl;

    printTest("deq", deq);
}