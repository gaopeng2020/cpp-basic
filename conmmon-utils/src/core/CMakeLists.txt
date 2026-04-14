# Core 模块配置
add_library(common-utils-core OBJECT)

target_sources(common-utils-core PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Core.cpp
)

target_include_directories(common-utils-core PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(common-utils-core PRIVATE
        common-utils-log
)
