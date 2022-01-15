#include "MapTest.h"

void pairConstructTest()
{
    // 调用构造函数 1，也就是默认构造函数
    pair<string, double> pair1;
    pair1.first = "pair1";
    pair1.second = 0.0;

    // 调用第 2 种构造函数
    pair<string, string> pair2("pair2", "pair构造函数方式2");

    // 调用拷贝构造函数
    pair<string, string> pair3(pair2);

    //调用移动构造函数 : 推荐
    pair<string, int> pair4(make_pair("C++", 4));

    cout << "pair1: " << pair1.first << " " << pair1.second << endl;
    cout << "pair2: " << pair2.first << " " << pair2.second << endl;
    cout << "pair3: " << pair3.first << " " << pair3.second << endl;
    cout << "pair4: " << pair4.first << " " << pair4.second << endl;
}
void constructTest()
{
    //默认构造函数，可以创建出一个空的 map 容器
    map<string, int> map1;

    //列表初始化构造
    map<string, int> map2{{"str1", 1}, {"str3", 3}, {"str2", 2}};

    //列表初始化，列表通过make_pair构造
    map<string, int> map3{make_pair("str1", 1), make_pair("str2", 2), make_pair("str3", 3)};

    //拷贝构造
    map<string, int> map4(map3);

    //通过迭代器构造
    map<string, int> map5(++map3.begin(), map3.end());
    printTest("map5", map5);

    //根据容器内各键值对的键的大小，对所有键值对做升序排序。
    map<string, int, less<string>> map6(map2);
    printTest("map6", map6);
}

map<string, uint8_t> addElementsTest()
{
    map<string, uint8_t, less<string>> students;

    /*   1. t通过[]运算符赋值,
    当操作对象为 map 容器中已存储的键值对时，则借助 [ ] 运算符，既可以获取指定键对应的值，还能对指定键对应的值进行修改；
    反之，若 map 容器内部没有存储以 [ ] 运算符内指定数据为键的键值对，则使用 [ ] 运算符会向当前 map 容器中添加一个新的键值对。 */
    students["gaopeng"] = 18;
    students["xiaolei"] = 17;

    //inser 方法插入键值对pair
    pair<string, uint8_t> xiaoping("xiaoping", 18);
    students.insert(xiaoping);
    students.insert(make_pair("xiaomeng", 20));

    //一次插入多个
    pair<map<string, uint8_t>::iterator, bool> par = students.insert({"xiaoyan", 19});
    students.insert({{"xiaogao", 18}, {"xiaoya", 18}, {"xiaonan", 18}});
    /*该方法的返回值也是一个 pair 对象，其中 pair.first 为一个迭代器，pair.second 为一个 bool 类型变量：
    如果插入成功，insert() 方法会返回一个指向 map 容器中已插入键值对的迭代器,同时 bool 变量的值为 true；
    如果插入失败(key已存在)，返回一个迭代器指向 map 容器中和 val 具有相同键的那个键值对,同时 bool 变量的值为 false */
    cout << par.first->first << par.first->second << par.second << endl;

    //在指定的迭代器位置插入键值对
    // 引用传递一个键值对:pair<iterator,bool> insert (const value_type& val);
    students.insert(students.end(), xiaoping);

    //以右值引用的方式传临时键值对
    /*  template <class P>
        pair<iterator,bool> insert (P&& val); */
    students.insert(students.begin(), pair<string, uint8_t>{"xiaoya", 18});

    //向当前 map 容器中插入其它 map 容器指定区域内的所有键值对
    map<string, uint8_t> othermap{{"xiaogao", 18}, {"xiaoya", 18}, {"xiaonan", 18}};
    students.insert(othermap.begin(), othermap.end());

    /*和 insert() 方法相比，emplace() 和 emplace_hint() 方法的使用要简单很多，因为它们各自只有一种语法格式。其中，emplace() 方法的语法格式如下：
    template <class... Args>
        pair<iterator,bool> emplace (Args&&... args);
    参数 (Args&&... args) 指的是，这里只需要将创建新键值对所需的数据作为参数直接传入即可，此方法可以自行利用这些数据构建出指定的键值对。
    另外，该方法的返回值也是一个 pair 对象，與inser一樣 */
    students.emplace("xiaowang", 16);
    students.emplace("xiaoxi", 16);

    /*  emplace_hint() 方法的功能和 emplace() 类似，其语法格式如下：
    template <class... Args>
        iterator emplace_hint (const_iterator position, Args&&... args);
    显然和 emplace() 语法格式相比，有以下 2 点不同：
    该方法不仅要传入创建键值对所需要的数据，还需要传入一个迭代器作为第一个参数，指明要插入的位置（新键值对键会插入到该迭代器指向的键值对的前面）；
    该方法的返回值是一个迭代器，而不再是 pair 对象。当成功插入新键值对时，返回的迭代器指向新插入的键值对；
    反之，如果插入失败，则表明 map 容器中存有相同键的键值对，返回的迭代器就指向这个键值对。 */
    students.emplace_hint(students.begin(), "xiaohua", 15);
    students.emplace_hint(students.begin(), "xiaoxi", 15);

    return students;
}

void iteratorTest(const map<string, uint8_t> &other)
{
    /*     map支持常规迭代器：
    begin()	返回指向容器中第一个（注意，是已排好序的第一个）键值对的双向迭代器。如果 map 容器用 const 限定，则该方法返回的是 const 类型的双向迭代器。
    end()	返回指向容器最后一个元素（注意，是已排好序的最后一个）所在位置后一个位置的双向迭代器，通常和 begin() 结合使用。如果 map 容器用 const 限定，则该方法返回的是 const 类型的双向迭代器。
    rbegin()	返回指向最后一个（注意，是已排好序的最后一个）元素的反向双向迭代器。如果 map 容器用 const 限定，则该方法返回的是 const 类型的反向双向迭代器。
    rend() 	返回指向第一个（注意，是已排好序的第一个）元素所在位置前一个位置的反向双向迭代器。如果 map 容器用 const 限定，则该方法返回的是 const 类型的反向双向迭代器。
    cbegin()	和 begin() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改容器内存储的键值对。
    cend() 	和 end() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改容器内存储的键值对。
    crbegin() 	和 rbegin() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改容器内存储的键值对。
    crend() 	和 rend() 功能相同，只不过在其基础上，增加了 const 属性，不能用于修改容器内存储的键值对。 */

    //除了常规迭代，map 类模板中还提供有 lower_bound(key) 和 upper_bound(key) 成员方法，更多用于 multimap 容器，在 map 容器中很少用到。
    // lower_bound(key) 返回的是指向第一个键不小于 key 的键值对的迭代器；
    map<std::string, uint8_t>::const_iterator low_it = other.lower_bound("xiaoping");
    cout << "lower_bound, Name = " << low_it->first << " age = " << low_it->second << endl;

    // upper_bound(key) 返回的是指向第一个键大于 key 的键值对的迭代器；
    map<std::string, uint8_t>::const_iterator upper_it = other.upper_bound("xiaolei");
    cout << "upper_bound, Name = " << upper_it->first << " age = " << upper_it->second << endl;

    /*     equal_range(key) 成员方法可以看做是 lower_bound(key) 和 upper_bound(key) 的结合体，该方法会返回一个 pair 对象，
    其中的 2 个元素都是迭代器类型，其中 pair.first 实际上就是 lower_bound(key) 的返回值，而 pair.second 则等同于 upper_bound(key) 的返回值。 */
    std::pair<std::map<std::string, uint8_t>::const_iterator, std::map<std::string, uint8_t>::const_iterator> equalRange = other.equal_range("gaopeng");
    for (auto it = equalRange.first; it != equalRange.second; ++it)
    {
        cout << "name = " << it->first << " age = " << it->second << endl;
    }
}

void accessDeleteTest(map<string, uint8_t> &other)
{
    /*  通过[] 获取，只有当 map 容器中确实存有包含该指定键的键值对，借助重载的 [ ] 运算符才能成功获取该键对应的值；
    反之，若当前 map 容器中没有包含该指定键的键值对，则此时使用 [ ]则成了向该 map 容器中增添一个键值对。
    其中，该键值对的键用 [ ] 运算符中指定的键，其对应的值取决于 map 容器规定键值对中值的数据类型，如果是基本数据类型，则值为 0；
    如果是 string 类型，其值为 ""，即空字符串（即使用该类型的默认值作为键值对的值）。 */
    cout << "[gaopeng] = " << unsigned(other["gaopeng"]) << endl;

    //还可以通过at()获取
    cout << " map.at(xiaonan) = " << unsigned(other.at("xiaonan")) << endl;

    //通过find方法，该方法返回的是一个迭代器，即如果查找成功，该迭代器指向查找到的键值对；反之，则指向 map 容器最后一个键值对之后的位置
    map<std::string, uint8_t>::iterator it = other.find("xiaoya");
    cout << "map.find(xiaoya) Name = " << it->first << ", age = " << unsigned(it->second) << endl;

    //如果以上方法都不适用，我们还可以遍历整个 map 容器
    for (auto it = other.begin(); it != other.end(); ++it)
    {
        //compare返回值为<0,=0, >0 加上“非”的条件，即等于0才能正确匹配
        if (!it->first.compare("xiaoya"))
        {
            cout << "map iterator and compare, Name = " << it->first << ", age = " << unsigned(it->second) << endl;
        }
    }
}