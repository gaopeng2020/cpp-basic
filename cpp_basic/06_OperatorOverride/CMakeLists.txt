project(CppBasic.06_OperatorOverrideTest VERSION 0.1.0)

#添加头文件索引位置
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

#查找根目录下所有的cpp文件
aux_source_directory(. ROOT_SRCS)
add_executable(${PROJECT_NAME} ${ROOT_SRCS})

# 如果模板的声明与实现分开了，通过链接库文件将模板的实现类添加到当前工程
# add_subdirectory(${PROJECT_SOURCE_DIR}/template)
# add_executable(OperatorOverrideTest ${ROOT_SRCS} src/OperatorOverride.cpp)
# target_link_libraries(${PROJECT_NAME} OperatorOverride)