#pragma once
#include <iostream>
#include <utility>
#include <map>
#include <typeinfo>
using namespace std;

/* 关联式容器存储的是“键值对”，考虑到“键值对”并不是普通类型数据，C++ STL 标准库提供了 pair 类模板，其专门用来将 2 个普通元素 
first 和 second（可以是 C++ 基本数据类型、结构体、类自定的类型）创建成一个新元素<first, second>
pair 类模板定义在<utility>头文件中，C++提供以下几种构造函数：
 */
void pairConstructTest();

/* map 容器存储的都是 pair 对象，也就是用 pair 类模板创建的键值对。map 容器存储的各个键值对，键的值既不能重复也不能被修改。
通常情况下，map 容器中存储的各个键值对都选用 string 字符串作为键的类型。
在使用 map 容器存储多个键值对时，该容器会自动根据各键值对的键的大小，按照既定的规则进行排序。
默认情况下，map 容器选用std::less<T>排序规则（其中 T 表示键的数据类型），其会根据键的大小对所有键值对做升序排序
我们可以手动指定 map 容器的排序规则，既可以选用 STL 标准库中提供的其它排序规则（比如std::greater<T>），也可以自定义排序规则。 */

/* 容器的模板定义如下：
template < class Key,                                     // 指定键（key）的类型
           class T,                                       // 指定值（value）的类型
           class Compare = less<Key>,                     // 指定排序规则
           class Alloc = allocator<pair<const Key,T> >    // 指定分配器对象的类型
           > class map; */

/* map构造函数
map<t1,T2>mapT;//map默认构造函数
map(const map &mp);//拷贝构造函数
map& operator=(const map &mp);//重载等号操作符
 */

/* map插入数据元素操作
operator[]	map容器重载了 [] 运算符，只要知道 map 容器中某个键值对的键的值，就可以向获取数组中元素那样，通过键直接获取对应的值。
at(key)	找到 map 容器中 key 键对应的值，如果找不到，该函数会引发 out_of_range 异常。
map.insert(...);//往容器插入元素，返回pair<iterator,bool>
    map<int,string>mapStu;
    mapStu.insert(pair<int,string>(22,"小恒"))；//第一种，通过pair的方式插入对象
    mapStu.insert(make_pair(22,"小恒"))；//第二种，通过pair的方式插入对象
    mapStu.insert(map<int,string>::value_type(22,"小恒"));//第三种，通过value_type的方式插入对象
    mapStu[2] = "小萌"；//第四种，通过数组的方式插入值
    mapStu[3] = "小印"；
emplace()	在当前 map 容器中的指定位置处构造新键值对。其效果和插入键值对一样，但效率更高。
emplace_hint()	在本质上和 emplace() 在 map 容器中构造新键值对的方式是一样的，不同之处在于，使用者必须为该方法提供一个指示键值对生成位置的迭代器，并作为该方法的第一个参数。
 */

/* map删除操作
clear();//删除所有元素
erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器
erase(begin,end);//删除区间【begin，end】的所有元素，返回下一个元素的迭代器
erase(keyElem);//删除容器中key为keyElem的对组
swap()	交换 2 个 map 容器中存储的键值对，这意味着，操作的 2 个键值对的类型必须相同。
 */

/* map查找操作
find(key);//查找键key是否存在，若存在，返回该键的元素的迭代器；若不存在，返回map.end（）；
count(keyElem);//返回容器中key为keyElem的对组个数，对map来说，值可能为0或1，对于multimap来说，值可能大于1
lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器
upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器
equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器
count(key)	在当前 map 容器中，查找键为 key 的键值对的个数并返回。注意，由于 map 容器中各键值对的键的值是唯一的，因此该函数的返回值最大为 1。
 */

/* map大小操作
size();//返回元素中元素的数目
empty() 若容器为空，则返回 true；否则 false。
max_size()	返回 map 容器所能容纳键值对的最大个数，不同的操作系统，其返回值亦不相同。
 */
void constructTest();

map<string, uint8_t> addElementsTest();

void iteratorTest(const map<string, uint8_t> &);

void accessDeleteTest(map<string, uint8_t> &);

template <typename T, typename U>
void printTest(string mapName, map<T, U> &map)
{
    for (auto it = map.begin(); it != map.end(); ++it)
        if (typeid(U) == typeid(uint8_t))
            cout << mapName << " [ key = " << it->first << ", value = " << unsigned(it->second) << " ]" << endl;
        else
            cout << mapName << " [ key = " << it->first << ", value = " << it->second << " ]" << endl;
}

/* multimap 容器具有和 map 相同的特性，即 multimap 容器也用于存储 pair<const K, T> 类型的键值对（其中 K 表示键的类型，T 表示值的类型），
其中各个键值对的键的值不能做修改；并且，该容器也会自行根据键的大小对存储的所有键值对做排序操作。和 map 容器的区别在于，
multimap 容器中可以同时存储多（≥2）个键相同的键值对。和 map 容器一样，实现 multimap 容器的类模板也定义在<map>头文件 */
