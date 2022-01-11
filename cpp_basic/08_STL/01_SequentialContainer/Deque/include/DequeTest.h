/* 当需要向序列两端频繁的添加或删除元素时，应首选 deque 容器
deque 容器和 vecotr 容器有很多相似之处，比如：
    deque 容器也擅长在序列尾部添加或删除元素（时间复杂度为O(1)），而不擅长在序列中间添加或删除元素。
    deque 容器也可以根据需要修改自身的容量和大小。
    和 vector 不同的是，deque 还擅长在序列头部添加或删除元素，所耗费的时间复杂度也为常数阶O(1)。
    并且更重要的一点是，deque 容器中存储元素并不能保证所有元素都存储到连续的内存空间中。 */

/* deque 构造函数
deque<T>deqT;//默认构造形式
deque(begin,end);//构造函数将【begin，end】区间的元素拷贝给自身
deque(n,elem);//构造函数将n个elem拷贝给自身
deque& operator=(const deque &deq);//重载等号操作符
deque(const deque &deq);//拷贝构造函数 */

/* deque大小操作
deque.size();//返回容器中元素的个数
deque.empty();//判断容器是否为空
shrink _to_fit()	将内存减少到等于当前元素实际所使用的大小。
deque.resize(num,elem);//重新指定容器长度为num，若容器变长，则以elem值填充新位置，
未指定elem值则以默认值0填充，若容器变短，则末尾超出容器长度的元素被删除 */

/* deque数据存取,访问Deque容器
at(int idx);//返回索引idx所指的数据，如果idx越界，抛出out_of_range异常
operator[];//返回索引idx所指的数据，越界时，运行直接报错
front();//返回容器中第一个数据元素
back();//返回容器中最后一个数据元素 */

/* deque赋值操作
assign(begin,end);//将【begin，end】区间的数据拷贝赋值给自身
assign(n,elem);//将n个elem拷贝赋值给自身
swap(deq);//将deq与本身的元素互换 */

/* deque双端插入和删除操作
insert(int pos, elem); //在pos位置插入元素elem
emplace()	在指定的位置直接生成一个元素。
emplace_front()	在容器头部生成一个元素。和 push_front() 的区别是，该函数直接在容器头部构造元素，省去了复制移动元素的过程。
emplace_back()	在容器尾部生成一个元素。和 push_back() 的区别是，该函数直接在容器尾部构造元素，省去了复制移动元素的过程
push_back(elem);//在容器尾部添加一个数据
push_front(elem);//在容器头部插入一个数据*/

/* deque删除操作
pop_back();//删除容器最后一个数据
pop_front();//删除容器第一个数据 
clear();//移除容器的所有数据
erase(begin,end);//删除【begin，end】区间的所有元素
erase(int pos);//删除pos位置的数据
 */
#pragma once
#include <iostream>
#include <deque>
#include <string>
#include <array>
#include <list>
#include <algorithm>
using namespace std;

void constructTest();

deque<int> addElementsTest();

void iteratorTest(deque<int> &deq);

void accessDeleteTest(deque<int> &deq);

template <typename T>
void printTest(string name, deque<T> &deq)
{
    int index = 0;
    auto begin = deq.begin();
    while (begin != deq.end())
    {
        cout << name + "[" << index << "] = " << *begin << endl;
        ++begin;
        index++;
    }
    cout << "==============================================" << endl;
}