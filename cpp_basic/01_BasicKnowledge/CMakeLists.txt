cmake_minimum_required(VERSION 3.0.0)

# string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})
# set (TARGET_NAME ${CURRENT_FOLDER})
# project(${CURRENT_FOLDER})
project(01_BasicKnowledge 
        VERSION 1.0.0
        DESCRIPTION "OneFLOW opensource CFD"
        HOMEPAGE_URL "https://github.com/"
        LANGUAGES CXX
)

#定义C++版本
add_definitions(-std=c++17)
#定义调试gdb信息 For Linux
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLASS} -O0 -Wall -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLASS} -O3 -Wall")

set(CMAKE_INSTALL_PREFIX ../install)

#设置生成的二进制文件存放位置
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#向工程添加头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)

#查找root目录下所有的cpp文件，并将其保存在变量ROOT_SRCS中
aux_source_directory(./ ROOT_SRCS)

#查找src目录下所有的cpp文件，并将其保存在变量SRC_SRCS中
aux_source_directory(./src SRC_SRCS)

# 编译SRC变量所代表的源代码文件，生成BasicKnowledge可执行文件
# add_executable(${PROJECT_NAME} ${ROOT_SRCS} ${SRC_SRCS})

add_library(${PROJECT_NAME} SHARED ${SRC_SRCS})


include(GNUInstallDirs)

#header files
file(GLOB HEADERS include/01_BasicKnowledge.h)
#intall header files
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${CMAKE_PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} EXPORT ${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/testHello)
#generate testHelloConfig.cmake
# configure_file(<input> <output>) 该命令将 <input> 指定的文件拷贝为 <output>指定的文件，
# 并将 <input> 文件中 @VAR@ 或 ${VAR} 的位置替换为使用 configure_file() 命令的当前 CMakeLists.txt 中变量 VAR 的值。
# 该命令用于外部文件获取 CMakeLists.txt 文件中变量的值。
configure_file(./cmake/TestHelloConfig.cmake.in ${PROJECT_BINARY_DIR}/testHelloConfig.cmake @ONLY)
# #install testHelloConfig.cmake
install(FILES ${PROJECT_BINARY_DIR}/testHelloConfig.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/testHello)
