cmake_minimum_required(VERSION 3.0.0)

project(VarString)

#定义C++版本
add_definitions(-std=c++17)
#定义调试gdb信息
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLASS} -O0 -Wall -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLASS} -O3 -Wall")

#设置生成的二进制文件存放位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#向工程添加头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)

#查找root目录下所有的cpp文件，并将其保存在变量ROOT_SRCS中
aux_source_directory(./ ROOT_SRCS)

#查找src目录下所有的cpp文件，并将其保存在变量SRC_SRCS中
aux_source_directory(./src SRC_SRCS)

# 编译SRC变量所代表的源代码文件，生成BasicKnowledge可执行文件
add_executable(${PROJECT_NAME} ${ROOT_SRCS} ${SRC_SRCS})