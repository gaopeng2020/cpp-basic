#pragma once
#include <iostream>
#include <list>
#include <array>
#include <string>
#include <algorithm>
using namespace std;

/* STL list容器，又称双向链表容器，即该容器的底层是以双向链表的形式实现的。
这意味着，list 容器中的元素可以分散存储在内存空间里，而不是必须存储在一整块连续的内存空间中。
ist 容器中各个元素的前后顺序是靠指针来维系的，每个元素都配备了 2 个指针，分别指向它的前一个元素和后一个元素。
其中第一个元素的前向指针总为 null，因为它前面没有元素；同样，尾部元素的后向指针也总为 null。

，list 容器具有一些其它容器（array、vector 和 deque）所不具备的优势，
即它可以在序列已知的任何位置快速插入或删除元素（时间复杂度为O(1)）。并且在 list 容器中移动元素，也比其它容器的效率高。
list 容器的缺点是，它不能像 array 和 vector 那样，通过位置直接访问元素。
需要从容器中第一个元素或最后一个元素开始遍历容器，直到找到该位置。
实际场景中，如何需要对序列进行大量添加或删除元素的操作，而直接访问元素的需求却很少，这种情况建议使用 list 容器存储序列。 */

/* list构造函数
list<T>lstT;//list采用模板类实现对象的默认构造函数
list(begin,end);//构造函数将【begin，end】区间中的元素拷贝给自身
list(int n,elem);//拷贝函数将n个elem拷贝给自身
list(const list &lst);//拷贝构造函数 */

/* 容器可用的成员函数
begin()	返回指向容器中第一个元素的双向迭代器。
end()	返回指向容器中最后一个元素所在位置的下一个位置的双向迭代器。
rbegin()	返回指向最后一个元素的反向双向迭代器。
rend()	返回指向第一个元素所在位置前一个位置的反向双向迭代器。
cbegin()	和 begin() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。
cend()	和 end() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。
crbegin() 	和 rbegin() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。
crend()	和 rend() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。

empty()	判断容器中是否有元素，若无元素，则返回 true；反之，返回 false。
size()	返回当前容器实际包含的元素个数。
max_size()	返回容器所能包含元素个数的最大值。这通常是一个很大的值，一般是 232-1，所以我们很少会用到这个函数。
resize()	调整容器的大小。

list数据元素插入和删除操作:
front()	返回第一个元素的引用。
back()	返回最后一个元素的引用。

assign()	用新元素替换容器中原有内容。
emplace_front()	在容器头部生成一个元素。该函数和 push_front() 的功能相同，但效率更高。
emplace_back()	在容器尾部直接生成一个元素。该函数和 push_back() 的功能相同，但效率更高。
push_front()	在容器头部插入一个元素。
push_back()	在容器尾部插入一个元素。
emplace()	在容器中的指定位置插入元素。该函数和 insert() 功能相同，但效率更高。
insert() 	在容器中的指定位置插入元素。
splice()	将一个 list 容器中的元素插入到另一个容器的指定位置。

pop_front()	删除容器头部的一个元素。
pop_back()	删除容器尾部的一个元素。
erase()	删除容器中一个或某区域内的元素。
clear()	删除容器存储的所有元素。
remove(val)	删除容器中所有等于 val 的元素。
remove_if()	删除容器中满足条件的元素。
unique()	删除容器中相邻的重复元素，只保留一个。

swap()	交换两个容器中的元素，必须保证这两个容器中存储的元素类型是相同的。
merge()	合并两个事先已排好序的 list 容器，并且合并之后的 list 容器依然是有序的。

list反转排列排序
sort()	通过更改容器中元素的位置，将它们进行排序。
reverse()	反转容器中元素的顺序。 */

void constructTest();

list<int> addElementsTest();

void iteratorTest();

void accessDeleteTest();

template <typename T>
void printTest(string listName, list<T> &list)
{
    int index = 0;
    auto begin = list.begin();
    while (begin != list.end())
    {
        cout << listName + "[" << index << "] = " << *begin << endl;
        ++begin;
        index++;
    }
    cout << "==============================================" << endl;
}
