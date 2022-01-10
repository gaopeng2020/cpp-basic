cmake_minimum_required(VERSION 3.0.0)
project(MyArrayTest VERSION 0.1.0)
#设置项目信息
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#设置可执行文件的搜索路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#添加头文件索引位置
include_directories(${PROJECT_SOURCE_DIR}/include)

#查找根目录下所有的cpp文件
aux_source_directory(./ ROOT_SRCS)
add_executable(${PROJECT_NAME} ${ROOT_SRCS})





