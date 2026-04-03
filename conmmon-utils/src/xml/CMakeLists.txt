# Xlsx 模块配置
add_library(common-utils-xml OBJECT)

target_sources(common-utils-xml PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Xml.cpp
)

target_include_directories(common-utils-xml PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(common-utils-xml PRIVATE
        glog::glog
        tinyxml2::tinyxml2
#        common-utils-core
)
