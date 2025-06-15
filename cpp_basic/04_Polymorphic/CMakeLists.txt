project(CppBasic.04_Polymorphic VERSION 0.1.0)

#向工程添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#查找current目录下所有的cpp文件
aux_source_directory(. SRC_SRCS)

# 并将src目录下的所有的cpp文件加入到executable
add_executable(${PROJECT_NAME} ${SRC_SRCS})

# 向当前工程添加存放源文件的子目录，子目录中需要中一个cMakeLists.txt
add_subdirectory(src)
# 添加共享链接库
target_link_libraries(${PROJECT_NAME} Employee)


