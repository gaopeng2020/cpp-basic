//
// Created by gaopeng on 2025/6/26.
//

#ifndef SETTEST_H
#define SETTEST_H
/*
 *set 容器中各键值对的键 key 和值 value 是相等的，根据 key 排序，也就等价为根据 value 排序。
 *使用 set 容器存储的各个元素的值必须各不相同。C++ 标准为了防止用户修改容器中元素的值，对所有可能会实现此操作的行为做了限制，
 *使得在正常情况下，用户是无法做到修改 set 容器中元素的值
 *set 容器的类模板定义
* template < class T,                        // 键 key 和值 value 的类型
           class Compare = less<T>,        // 指定 set 容器内部的排序规则
           class Alloc = allocator<T>      // 指定分配器对象的类型
           > class set;

*set 容器的构造方法
*1.调用默认构造函数，创建空的 set 容器：std::set<std::string> myset;
*2.set 类模板还支持在创建 set 容器的同时，对其进行初始化std::set<std::string> myset{"http://c.biancheng.net/java/",
                            "http://c.biancheng.net/stl/",
                            "http://c.biancheng.net/python/"};
*3.set 类模板中还提供了拷贝（复制）构造函数;std::set<std::string> copyset(myset);
*等同于//std::set<std::string> copyset = myset
*4.C++ 11 标准还为 set 类模板新增了移动构造函数，利用临时的 set(函数返回值) 容器为其初始化
*5. set 类模板还支持取已有 set 容器中的部分元素，来初始化新 set 容器 std::set<std::string> copyset(++other.begin(), other.end());
 */
#include <set>
// void
#endif //SETTEST_H
