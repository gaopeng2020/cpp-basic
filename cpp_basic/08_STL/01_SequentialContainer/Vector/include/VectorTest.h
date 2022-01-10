/* vector 实现的是一个动态数组，可以进行元素的插入和删除，在此过程中，vector会动态调整所占用的内存空间。
vector 常被称为向量容器，因为该容器擅长在尾部插入或删除元素，在常量时间内就可以完成，时间复杂度为O(1)；
而对于在容器头部或者中部插入或删除元素，则花费时间要长一些（移动元素需要耗费时间），时间复杂度为线性阶O(n)。 */

/**
 * vector容器构造方式：
 * 定义一个空的vector：std::vector<double> vector;
 * 在创建 vector 容器时，也可以指定元素个数vector：std::vector<double> vect(20);
 * 构建的同时指定初始值以及元素个数 std::vector<int> vect {2, 3, 5, 7, 11, 13, 17, 19};
 * 通过其他的vector拷贝构造 std::vector<int> vect(std::vector<int> otherVect);
 * 通过数组拷贝构造 int array[]={1,2,3};
                   std::vector<int>values(array, array+2);//values 将保存{1,2}
 *  通过迭代器构造 std::vector<int>value2(std::begin(value1),std::begin(value1)+3)
  */

/*  vector 容器的成员函数
函数成员	函数功能
begin()	返回指向容器中第一个元素的迭代器。
end()	返回指向容器最后一个元素所在位置后一个位置的迭代器，通常和 begin() 结合使用。
rbegin()	返回指向最后一个元素的迭代器。
rend()	返回指向第一个元素所在位置前一个位置的迭代器。
cbegin()	和 begin() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。
cend()	和 end() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。
crbegin()	和 rbegin() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。
crend()	和 rend() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。

size()	返回实际元素个数。
max_size()	返回元素个数的最大值。这通常是一个很大的值，一般是 232-1，所以我们很少会用到这个函数。
resize()	改变实际元素的个数。
capacity()	返回当前容量。
empty()	判断容器中是否有元素，若无元素，则返回 true；反之，返回 false。
reserve()	增加容器的容量。
shrink _to_fit()	将内存减少到等于当前元素实际所使用的大小。

operator[ ]	重载了 [ ] 运算符，可以向访问数组中元素那样，通过下标即可访问甚至修改 vector 容器中的元素。
at()	使用经过边界检查的索引访问元素。
front()	返回第一个元素的引用。
back()	返回最后一个元素的引用。
data()	返回指向容器中第一个元素的指针。
assign()	用新元素替换原有内容。
push_back()	在序列的尾部添加一个元素。
pop_back()	移出序列尾部的元素。
insert()	在指定的位置插入一个或多个元素。
erase()	移出一个元素或一段元素。
clear()	移出所有的元素，容器大小变为 0。
swap()	交换两个容器的所有元素。
emplace()	在指定的位置直接生成一个元素。
emplace_back()	在序列尾部生成一个元素。 */

#include <iostream>
#include <vector>
#include <array>
#include <string>
using namespace std;

void constructTest();

void addElementsTest();

void insertElementTesr();

void iteratorTest();

void accessDeleteTest();

template <typename T>
void printTest(string vectName, vector<T> &vect){
  {
  for (size_t i = 0; i < vect.size(); i++)
  {
    cout <<vectName+"["<<i<<"] = "<< vect.at(i) << endl;
  }
}
}