# Core 模块配置
add_library(common-utils-core OBJECT)

target_sources(common-utils-core PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Core.cpp
)

target_include_directories(common-utils-core PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)
#关键：告诉编译器正在导出符号
target_compile_definitions(common-utils-core PRIVATE
        common_utils_EXPORTS
)
#target_link_libraries(common-utils-core PRIVATE
#        common-utils-log
#)
