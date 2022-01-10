#设置头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

#设置Static lib文件存放位置
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#设置Dynamic lib文件存放位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

#生成库文件
# add_library(Employee STATIC ${SRC})
add_library(Employee SHARED ${SRC})