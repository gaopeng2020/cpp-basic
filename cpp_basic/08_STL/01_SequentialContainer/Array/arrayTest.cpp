#include "arrayTest.h"

void constructTest() {
    array<uint16_t, 10> array1{};
    cout << "array1 iterator" << endl;
    for (size_t i = 0; i < array1.size(); i++) {
        array1[i] = i;
        cout << "array1[" << i << "] = " << array1.at(i) << endl;
    }

    array<int, 10> array2{0};
    int value = 0;
    for (auto it = array2.begin(); it < array2.end(); ++it) {
        *it = value;
        value++;
    }
    // auto it = array2.begin(),end = array2.end();
    // while (it!=end)
    // {
    //     *it = value;
    //     value++;
    //     ++it;
    // }
    cout << "array2 iterator" << endl;
    for (size_t i = 0; i < array2.size(); i++) {
        cout << "array2[" << i << "] = " << array2.at(i) << endl;
    }

    array<int16_t, 10> array3{255, 245, 235, 225, 215, 205, 195, 185, 175, 165};
    cout << "array3 iterator" << endl;
    for (short &it: array3) {
        cout << it << endl;
    }
}

void addElementsTest() {
    array<double, 11> array1{0.0, 1.0, 2.0, 3.0, 4.0};
    array1[5] = 5.0f;
    array1.at(6) = 6.0f;

    auto begin = array1.begin();
    *(begin + 7) = 7.0f;
    *(begin + 8) = 8.0f;

    auto end = array1.end();
    *(--end) = 10.0f;
    *(--end) = 9.0f;

    for (size_t i = 0; i < array1.size(); i++) {
        cout << "array1[" << i << "] = " << array1.at(i) << endl;
    }

    array<double, 11> array2 = {10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
    swap(array1, array2);

    cout << "====================for(auto it: array1)Start=========================" << endl;
    for (auto it: array1) {
        cout << it << endl;
    }
    cout << "======================for(auto it: array1)End=======================" << endl;
}

void iteratorTest() {
    array<int, 5> values{0};
    // array<int, 5> values{1, 2, 3, 4, 5};
    int i = 1;
    auto first = values.begin();
    auto last = values.end();
    //初始化 values 容器为{1,2,3,4,5}
    while (first != last) {
        *first = i;
        i++;
        ++first;
    }

    first = values.begin();
    while (first != last) {
        cout << *first << " ";
        ++first;
    }
    cout << endl;
}

void removeElementTest() {
    //std::array 是一个 固定大小的容器，其大小在编译时确定，因此 不支持直接添加或删除元素
    std::array<int, 5> values{};
    //初始化 values 容器为 {0,1,2,3,4}
    for (size_t i = 0; i < values.size(); i++) {
        values.at(i) = i;
    }

    cout << "values[0] = " << values.front() << endl;
    cout << "values[1] = " << values.at(1) << endl;
    cout << "values[2] = " << values[2] << endl;
    cout << "values[3] = " << get<3>(values) << endl;
    cout << "values[4] = " << values.back() << endl;

//    可以用 std::remove 将待删除元素移到末尾，并调整逻辑范围
    auto pInt = std::remove(values.begin(), values.end(), 2);

    //如果容器不为空，则通过迭代器指针输出容器中所有的元素
    for (auto val = values.begin(); val < values.end(); val++) {
        cout << *val << " ";
    }
    cout << endl;
}
