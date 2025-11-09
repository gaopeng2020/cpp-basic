//
// Created by gaopeng on 2025/6/22.
//
/*关于元组的使用有三个核心的函数：
    1. `std::make_tuple`: 构造元组
    2. `std::get`: 获得元组某个位置的值
    3. `std::tie`: 元组拆包
*/
#include <tuple>
#include <iostream>

auto get_student(int id) {
    // 返回类型被推断为 std::tuple<double, char, std::string>

    if (id == 0)
        return std::make_tuple(3.8, 'A', "张三");
    if (id == 1)
        return std::make_tuple(2.9, 'C', "李四");
    if (id == 2)
        return std::make_tuple(1.7, 'D', "王五");
    return std::make_tuple(0.0, 'D', "null");
    // 如果只写 0 会出现推断错误, 编译失败
}

int main() {
    //构造turtle
    std::tuple<std::string, double, double, int> t("str123", 4.5, 6.7, 8);
    std::cout << std::get<std::string>(t) << std::endl;

    // 获得元组某个位置的值
    auto student = get_student(0);
    std::cout << "ID: 0, "
            << "GPA: " << std::get<0>(student) << ", "
            << "成绩: " << std::get<1>(student) << ", "
            << "姓名: " << std::get<2>(student) << '\n';

    // std::get` 除了使用常量获取元组对象外，C++14 增加了使用类型来获取元组中的对象：
    std::cout << std::get<double>(student) << std::endl;
    // std::cout << std::get<std::string>(student) << std::endl; // 非法, 引发编译期错误


    // 元组进行拆包
    double gpa;
    char grade;
    std::string name;

    std::tie(gpa, grade, name) = get_student(1);
    std::cout << "ID: 1, "
            << "GPA: " << gpa << ", "
            << "成绩: " << grade << ", "
            << "姓名: " << name << '\n';

    // 运行期索引
}
