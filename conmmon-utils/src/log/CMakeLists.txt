# log 模块配置
add_library(common-utils-log OBJECT)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
target_sources(common-utils-log PRIVATE ${SOURCES})

target_include_directories(common-utils-log PRIVATE
        ${CMAKE_SOURCE_DIR}/include/
)
