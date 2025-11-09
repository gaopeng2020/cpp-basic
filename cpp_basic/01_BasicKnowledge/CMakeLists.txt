project(CppBasic.01_BasicKnowledge
        VERSION 1.0.0
        DESCRIPTION "OneFLOW opensource CFD"
        LANGUAGES CXX
)

#向工程添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#查找root目录下所有的cpp文件，并将其保存在变量ROOT_SRCS中
file(GLOB_RECURSE SRCS "./*.cpp")

# 编译SRC变量所代表的源代码文件，生成BasicKnowledge可执行文件
 add_executable(${PROJECT_NAME} ${SRCS})

