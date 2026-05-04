#添加头文件索引位置
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}-visual-function src/AbstractComputer.cpp src/main.cpp)