cmake_minimum_required(VERSION 3.0.0)
project(Polymorphic VERSION 0.1.0)
add_definitions(-std=c++11)

#设置项目信息
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

#定义gdb调试信息
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLASS} -O0 -Wall -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLASS} -O3 -Wall")

#设置可执行文件的保存路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#向工程添加头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)

#查找current目录下所有的cpp文件
aux_source_directory(./ SRC_SRCS)

# 并将src目录下的所有的cpp文件加入到executable
add_executable(${PROJECT_NAME} ${SRC_SRCS})

# 向当前工程添加存放源文件的子目录，子目录中需要中一个cMakeLists.txt
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
# 添加共享链接库
target_link_libraries(${PROJECT_NAME} Employee)


