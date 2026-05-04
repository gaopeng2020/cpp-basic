#向工程添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 并将src目录下的所有的cpp文件加入到executable
file(GLOB_RECURSE SRCS "src/*.cpp")
#file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME}-polymorphic ${SRCS} main.cpp)



