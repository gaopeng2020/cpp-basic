#设置头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)

#生成库文件
add_library(Employee STATIC ${SRC})
#add_library(Employee SHARED ${SRC})