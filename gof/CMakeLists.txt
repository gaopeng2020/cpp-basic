cmake_minimum_required(VERSION 3.10)
project(gof LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加所有子项目
add_subdirectory(behavior/Strategy)
add_subdirectory(behavior/TemplateMethod)
add_subdirectory(creation/AbstractFactory)
add_subdirectory(creation/factorymethod)
