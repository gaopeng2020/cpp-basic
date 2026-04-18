# Xlsx 模块配置
add_library(common-utils-xlsx OBJECT)

target_sources(common-utils-xlsx PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/xlsx.cpp
)

target_include_directories(common-utils-xlsx PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)

target_link_libraries(common-utils-xlsx PRIVATE
        OpenXLSX::OpenXLSX
#        common-utils-core
#        common-utils-log
)
#关键：告诉编译器正在导出符号
target_compile_definitions(common-utils-xlsx PRIVATE
        common_utils_EXPORTS
)