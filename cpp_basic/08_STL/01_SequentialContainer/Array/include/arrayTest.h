/* array 容器以类模板的形式定义在 <array> 头文件，并位于命名空间 std 中.
namespace std{
    template <typename T, size_t N>
    class array;
} 
因此，在使用该容器之前，代码中需引入 <array> 头文件，并默认使用 std 命令空间，如下所示：
#include <array>
using namespace std;
在 array<T,N> 类模板中，T 用于指明容器中的存储的具体数据类型，
N 用于指明容器的大小，需要注意的是，这里的 N 必须是常量，不能用变量表示。
*/

/* array的构造方法：
std::array<double, 10> values;
std::array<double, 10> values {};
std::array<double, 10> values {0.5,1.0,1.5,,2.0};
 */

/* Array成员函数
成员函数	功能
begin()	返回指向容器中第一个元素的随机访问迭代器。
end()	返回指向容器最后一个元素之后一个位置的随机访问迭代器，通常和 begin() 结合使用。
rbegin()	返回指向最后一个元素的随机访问迭代器。
rend()	返回指向第一个元素之前一个位置的随机访问迭代器。
cbegin()	和 begin() 功能相同，只不过在其基础上增加了 const 属性，不能用于修改元素。
cend()	和 end() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。
crbegin()	和 rbegin() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。
crend()	和 rend() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改元素。

size()	返回容器中当前元素的数量，其值始终等于初始化 array 类的第二个模板参数 N。
max_size()	返回容器可容纳元素的最大数量，其值始终等于初始化 array 类的第二个模板参数 N。
empty()	判断容器是否为空，和通过 size()==0 的判断条件功能相同，但其效率可能更快。

at(n)	返回容器中 n 位置处元素的引用，该函数自动检查 n 是否在有效的范围内，如果不是则抛出 out_of_range 异常。
get() 全局函数，该重载函数的功能是访问容器中指定的元素，并返回该元素的引用。
front()	返回容器中第一个元素的直接引用，该函数不适用于空的 array 容器。
back()	返回容器中最后一个元素的直接应用，该函数同样不适用于空的 array 容器。
data()	返回一个指向容器首个元素的指针。利用该指针，可实现复制容器中所有元素等类似功能。

fill(val)	将 val 这个值赋值给容器中的每个元素。
array1.swap(array2)	或者swap(array1,array2)交换 array1 和 array2 容器中的所有元素，但前提是它们具有相同的长度和类型。 */

#include <iostream>
#include <array>
using namespace std;

void constructTest();

void addElementsTest();

void iteratorTest();

void accessDeleteTest();
