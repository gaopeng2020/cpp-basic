cmake_minimum_required(VERSION 3.15)

add_executable(${PROJECT_NAME}-test main.cpp)

target_link_libraries(${PROJECT_NAME}-test PRIVATE
        ${PROJECT_NAME}
)
# 设置包含目录
target_include_directories(${PROJECT_NAME}-test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

