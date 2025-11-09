project(CppBasic.STL_ListTest)

#向工程添加头文件搜索路径
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

file(GLOB_RECURSE SRC_LIST "./*.cpp")
# 编译SRC变量所代表的源代码文件，生成BasicKnowledge可执行文件
add_executable(${PROJECT_NAME} ${SRC_LIST})