project(CppBasic.03_Inheritance VERSION 0.1.0)

#向工程添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#查找root目录下所有的cpp文件，并将其保存在变量ROOT_SRCS中
aux_source_directory(./ ROOT_SRCS)

#查找src目录下所有的cpp文件，并将其保存在变量SRC_SRCS中
aux_source_directory(./src SRC_SRCS)

# 编译SRC变量所代表的源代码文件，生成BasicKnowledge可执行文件
add_executable(${PROJECT_NAME} ${ROOT_SRCS} ${SRC_SRCS})
