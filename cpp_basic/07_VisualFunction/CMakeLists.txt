project(CppBasic.07_VisualFunction VERSION 0.1.0)

#添加头文件索引位置
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#查找根目录下所有的cpp文件
#aux_source_directory(./ ROOT_SRCS)
file(GLOB SRC_LIST "src/*.cpp")       # 匹配 src/ 下的 .cpp 文件
#file(GLOB_RECURSE SRC_LIST "*.cpp") # 递归匹配所有子目录的 .cpp 文件
add_executable(${PROJECT_NAME} ${SRC_LIST})