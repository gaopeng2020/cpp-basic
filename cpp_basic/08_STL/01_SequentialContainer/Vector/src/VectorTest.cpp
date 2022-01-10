#include "VectorTest.h"
#include "Student.h"

void constructTest()
{
  vector<int16_t> vector1;
  for (size_t i = 0; i < 10; i++)
  {
    vector1.push_back(i);
    cout << "vector1[" << i << "] = " << vector1[i] << endl;
  }

  vector<double> vector2(10);
  for (size_t i = 0; i < vector2.size(); i++)
  {
    vector2.at(i) = 9 - i;
    cout << "vector2[" << i << "] = " << vector2.at(i) << endl;
  }

  vector<float> vector3 = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  // vector<float> vector3 {1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f,10.0f};
  printTest("vector3", vector3);

  vector<double> vector4(vector2);
  printTest("vector4", vector4);

  int array[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
  vector<int> vector5(array, array + 8);
  printTest("vector5", vector5);

  vector<int> vector6(vector5.rbegin(), vector5.rend());
  printTest("vector6", vector6);
}

void addElementsTest()
{

  vector<int> vect;
  /*push_back() 该成员函数的功能是在 vector 容器尾部添加一个元素
  emplace_back() 该函数是 C++ 11 新增加的，其功能和 push_back() 相同，都是在 vector 容器的尾部添加一个元素。
  push_back() 向容器尾部添加元素时，首先会创建这个元素，然后再将这个元素拷贝或者移动到容器中（如果是拷贝的话，事后会自行销毁先前创建的这个元素）；
  而 emplace_back() 在实现时，则是直接在容器尾部创建这个元素，省去了拷贝或移动元素的过程。 */
  vect.push_back(1);
  vect.emplace_back(3);
  vect.emplace_back(5);

  /* insert() 成员函数语法格式
语法格式	用法说明: pos 为指定插入位置的迭代器
iterator insert(pos,elem)	在迭代器 pos 指定的位置之前插入一个新元素elem，并返回表示新插入元素位置的迭代器。
iterator insert(pos,n,elem)	在迭代器 pos 指定的位置之前插入 n 个元素 elem，并返回表示第一个新插入元素位置的迭代器。
iterator insert(pos,first,last) 	在迭代器 pos 指定的位置之前，插入其他容器（不仅限于vector）中位于 [first,last) 
区域的所有元素，并返回表示第一个新插入元素位置的迭代器。
iterator insert(pos,initlist)	在迭代器 pos 指定的位置之前，插入初始化列表（用大括号{}括起来的多个元素，中间有逗号隔开）
中所有的元素，并返回表示第一个新插入元素位置的迭代器。 */
  vect.insert(vect.begin() + 1, 2);
  vect.insert(vect.begin() + 3, 4);
  vect.insert(vect.begin() + 5, 1, 6);

  array<int, 4> ary{7, 8, 9, 10};
  vect.insert(vect.end(), ary.begin(), ary.end());
  vect.insert(vect.end(), {11, 12, 3, 14, 15});

  /* emplace() 是 C++ 11 标准新增加的成员函数，用于在 vector 容器指定位置之前插入一个新的元素,比insert运行效率要高，建议使用
该函数的语法格式如下：
iterator emplace (const_iterator pos, args...);
其中，pos 为指定插入位置的迭代器；args... 表示与新插入元素的构造函数相对应的多个参数；该函数会返回表示新插入元素位置的迭代器。 */
  vect.emplace(vect.begin(), 0xff);
  vect.emplace(vect.begin() + 1, 0xffff);
  vect.at(0) = -1;
  vect[1] = 0;

  printTest("vect", vect);
}

void iteratorTest()
{
  /*   vector 支持迭代器的成员函数
成员函数	功能
begin()	返回指向容器中第一个元素的正向迭代器；如果是 const 类型容器，在该函数返回的是常量正向迭代器。
end()	返回指向容器最后一个元素之后一个位置的正向迭代器；如果是 const 类型容器，在该函数返回的是常量正向迭代器。此函数通常和 begin() 搭配使用。
rbegin()	返回指向最后一个元素的反向迭代器；如果是 const 类型容器，在该函数返回的是常量反向迭代器。
rend()	返回指向第一个元素之前一个位置的反向迭代器。如果是 const 类型容器，在该函数返回的是常量反向迭代器。此函数通常和 rbegin() 搭配使用。
cbegin()	和 begin() 功能类似，只不过其返回的迭代器类型为常量正向迭代器，不能用于修改元素。
cend()	和 end() 功能相同，只不过其返回的迭代器类型为常量正向迭代器，不能用于修改元素。
crbegin()	和 rbegin() 功能相同，只不过其返回的迭代器类型为常量反向迭代器，不能用于修改元素。
crend()	和 rend() 功能相同，只不过其返回的迭代器类型为常量反向迭代器，不能用于修改元素。 */
  vector<Student> students;
  Student xiaoming = {"xiaoming", 10, "Shool1"};
  students.emplace_back(xiaoming);
  Student xiaohua{"xiaohua", 12, "Shool2"};
  students.emplace_back(xiaohua);
  Student xiaohong("xiaohong", 13, "Shool3");
  students.emplace_back(xiaohong);
  Student *xiaolv = new Student("xiaolv", 14, "Shool3");
  students.emplace_back(*xiaolv);

  /*   i++是先取变量i，再将变量i值+1；而++i是先将变量i值+1，再取变量i。
  在循环遍历容器变量时，这两种方式的结果都是一样的，但是，本质的效率上有很大的区别;
  两种方式iterator遍历的次数是相同的，但在STL中效率不同，前++返回引用，后++返回一个临时对象, 
  it++是函数重载，所以编译器无法对其进行优化，所以每遍历一个元素，你就创建并销毁了一个无用的临时对象
  除了特殊需要和对内置类型外，基本都是使用++it来进行元素遍历的 */

  // begin() 和 end() 成员函数，它们分别用于指向「首元素」和「尾元素+1」 的位置
  for (auto it = students.begin(); it != students.end(); ++it)
  {
    printStudent(*it);
  }
  // 由 cbegin() 和 cend() 成员函数返回的迭代器，可以用来遍历容器内的元素，也可以访问元素，但是不能对所存储的元素进行修改
  auto begin = students.cbegin();
  cout << "Iterator vector by cbegin and cend" << endl;
  while (begin != students.cend())
  {
    printStudent(*begin);
    ++begin;
  }
  /*
 * 注意：
 * 1. 在初始化空的 vector 容器时，不能使用迭代器循环，因此begin()和end()指向同一个位置；
 * 2. vector 容器在申请更多内存的同时，容器中的所有元素可能会被复制或移动到新的内存地址，这会导致之前创建的迭代器失效。
 *    为了保险起见，每当 vector 容器的容量发生变化时，我们都要对之前创建的迭代器重新初始化一遍：
 */
}

void accessDeleteTest()
{
  vector<uint32_t> u16Vector{0xF, 0xFF, 0xFFF, 0xFFFF, 0xFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFFF};
  // 1. vector 容器可以向普通数组那样通过指定下标访问存储的元素；
  cout << "u16Vector[1] = " << u16Vector[1] << endl;

  // 2. vector 容器也提供了 at() 成员函数，当传给 at() 的索引会造成越界时，会抛出std::out_of_range异常，建议使用at();
  cout << "u16Vector.at(2) = " << u16Vector.at(2) << endl;
  // cout<<"u16Vector.at(10)"<<u16Vector.at(10)<<endl;

  // 3. 即 front() 和 back()，它们分别返回 vector 容器中第一个和最后一个元素的引用，利用这 2 个函数返回的引用，可以访问（甚至修改）容器中的首尾元素。
  u16Vector.front() = 0;
  cout << "u16Vector.front() = " << u16Vector.front() << endl;
  cout << "u16Vector.back() = " << u16Vector.back() << endl;

  // 4. vector 容器还提供了 data() 成员函数，该函数的功能是返回指向容器中首个元素的指针。通过该指针也可以访问甚至修改容器中的元素
  u16Vector.data();
  cout << "u16Vector.data()+1 = " << u16Vector.data() + 1 << endl;

  /*     删除 vector 容器元素的几种方式
  pop_back()	删除 vector 容器中最后一个元素，该容器的大小（size）会减 1，但容量（capacity）不会发生改变。
  erase(pos)	删除 vector 容器中 pos 迭代器指定位置处的元素，并返回指向被删除元素下一个位置元素的迭代器。该容器的大小（size）会减 1，但容量（capacity）不会发生改变。
  swap(beg) && pop_back()	先调用 swap() 函数交换要删除的目标元素和容器最后一个元素的位置，然后使用 pop_back() 删除该目标元素。
  erase(beg,end)	删除 vector 容器中位于迭代器 [beg,end)指定区域内的所有元素，并返回指向被删除区域下一个位置元素的迭代器。该容器的大小（size）会减小，但容量（capacity）不会发生改变。
  remove()	删除容器中所有和指定元素值相等的元素，并返回指向最后一个元素下一个位置的迭代器。值得一提的是，调用该函数不会改变容器的大小和容量。
  clear()	删除 vector 容器中所有的元素，使其变成空的 vector 容器。该函数会改变 vector 的大小（变为 0），但不是改变其容量。 */

  u16Vector.pop_back();
  u16Vector.erase(u16Vector.begin() + 1);
  printTest("u16Vector", u16Vector);
}