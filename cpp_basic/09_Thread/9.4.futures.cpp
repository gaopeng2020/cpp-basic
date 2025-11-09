//
// 7.4.futures.cpp
// chapter 7 parallelism and concurrency
// modern c++ tutorial
//
// created by changkun at changkun.de
// https://github.com/changkun/modern-cpp-tutorial
//

#include <iostream>
#include <thread>
#include <future>
/*
 * 期物（Future）表现为 `std::future`，它提供了一个访问异步操作结果的途径。
* 试想，如果我们的主线程 A 希望新开辟一个线程 B 去执行某个我们预期的任务，并返回我一个结果。
* 而这时候，线程 A 可能正在忙其他的事情，无暇顾及 B 的结果，所以我们会很自然的希望能够在某个特定的时间获得线程 B 的结果。
* `std::future` 可以用来获取异步任务的结果。
 */
int main() {
    // pack a lambda expression that returns 7 into a std::packaged_task
    std::packaged_task<int()> task([](){return 7;});
    // get the future of task
    std::future<int> result = task.get_future();    // run task in a thread
    std::thread(std::move(task)).detach();
    std::cout << "waiting...";
    result.wait(); // block until future has arrived
    // output result
    std::cout << "done!" << std:: endl << "future result is " << result.get() << std::endl;
    return 0;
}