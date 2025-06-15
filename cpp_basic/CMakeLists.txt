cmake_minimum_required(VERSION 3.15)
project("CppBasic")

#定义C++版本
add_definitions(-std=c++17)
#定义调试gdb信息 For Linux
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLASS} -O0 -Wall -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLASS} -O3 -Wall")

set(CMAKE_INSTALL_PREFIX ../install)

# Output Directories.
#=======================================================================================================================
#控制所有运行时文件（可执行文件、DLL 等）的输出目录。
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#仅控制可执行文件（.exe）的输出目录,旧版不推荐在新项目中使用
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
#设置Dynamic lib文件存放位置
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#设置Static lib文件存放位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(01_BasicKnowledge)
add_subdirectory(02_ClassEncapsulation)
add_subdirectory(03_Inheritance)
add_subdirectory(04_Polymorphic)
add_subdirectory(05_Template)
add_subdirectory(06_OperatorOverride)
add_subdirectory(07_VisualFunction)
#add_subdirectory(08_STL)