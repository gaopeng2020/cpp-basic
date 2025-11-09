//
// 7.2.critical.section.a.cpp
// chapter 7 parallelism and concurrency
// modern c++ tutorial
//
// created by changkun at changkun.de
// https://github.com/changkun/modern-cpp-tutorial
//

#include <iostream>
#include <mutex>
#include <thread>

int v = 1;
/*
* `std::mutex` 是 C++11 中最基本的互斥量类，可以通过构造 `std::mutex` 对象创建互斥量，
而通过其成员函数 `lock()` 可以进行上锁，`unlock()` 可以进行解锁。
但是在实际编写代码的过程中，最好不去直接调用成员函数，
因为调用成员函数就需要在每个临界区的出口处调用 `unlock()`，当然，还包括异常。
而 C++11 为互斥量提供了一个 RAII 机制的模板类 `std::lock_guard`。无论正常返回、还是在中途抛出异常，都会引发栈回溯，也就自动调用了 `unlock()`。
 */
void critical_section(int change_v) {
    static std::mutex mtx;
    std::lock_guard guard(mtx);

    // do contention operations
    v = change_v;

    // mtx will be destructed when exit this region
}

int main() {
    std::thread t1(critical_section, 2), t2(critical_section, 3);
    t1.join();
    t2.join();

    std::cout << v << std::endl;
    return 0;
}
