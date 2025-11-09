project(CppBasic.STL_MapTest VERSION 0.1.0)

#向工程添加头文件搜索路径
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

#查找root目录下所有的cpp文件，并将其保存在变量ROOT_SRCS中
file(GLOB_RECURSE SRC_LISR "./*.cpp")

# 编译SRC变量所代表的源代码文件，生成BasicKnowledge可执行文件
add_executable(${PROJECT_NAME} ${SRC_LISR})