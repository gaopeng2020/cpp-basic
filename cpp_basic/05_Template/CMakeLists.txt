project(CppBasic.05_MyArrayTest VERSION 0.1.0)

#添加头文件索引位置
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

#查找根目录下所有的cpp文件
file(GLOB SRC_LIST "*.cpp")
add_executable(${PROJECT_NAME} ${SRC_LIST})





