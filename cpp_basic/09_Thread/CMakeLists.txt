cmake_minimum_required(VERSION 3.15)
project(CppBasic.ThreadTest)

add_executable(ThreadTestDemo1 9.1.thread.basic.cpp)

add_executable(ThreadTestDemo2 9.2.critical.section.a.cpp)

add_executable(ThreadTestDemo3 9.3.critical.section.b.cpp)

add_executable(ThreadTestDemo4 9.4.futures.cpp)

add_executable(ThreadTestDemo5 9.5.producer.consumer.cpp)

add_executable(ThreadTestDemo6 9.6.atomic.cpp)

add_executable(ThreadTestDemo7 9.7.bad.example.cpp)

add_executable(ThreadTestDemo8 9.8.is.lock.free.cpp)

add_executable(ThreadTestDemo9 9.9.memory.order.cpp)