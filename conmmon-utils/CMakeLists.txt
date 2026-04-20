cmake_minimum_required(VERSION 3.20)
# ===================================== 用户配置区 =====================================
# 使用前请修改以下变量

# 1. 库的名称前缀（例如 Calculator -> Calculator）
set(LIB_NAME common-utils)

# 2. 库类型：SHARED（ON）或 STATIC（OFF）
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# 3. vcpkg 的 triplet 设置（根据您的工具链选择）
#  常用值：x64-windows、x64-linux 、x64-mingw-dynamic、x64-mingw-static
#set(VCPKG_TARGET_TRIPLET "x64-windows")
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic")
message(STATUS "Auto-detected VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

#定义Project，一般也无需修改
project(${LIB_NAME} VERSION 1.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 依赖库处理（直接使用 find_package，不再使用复杂的列表循环）
find_package(OpenXLSX CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)

# 5. 收集依赖目标（用于target_link_libraries）
# 注意：如果库提供了 CMake 目标，请使用带命名空间的名称（如 tinyxml2::tinyxml2）
set(DEP_TARGETS
        OpenXLSX::OpenXLSX
        tinyxml2::tinyxml2
)
# 6. 收集 find_dependency 代码（用于生成 Config.cmake）
set(DEP_FIND_CODE "")
string(APPEND DEP_FIND_CODE "find_dependency(OpenXLSX REQUIRED)\n")
string(APPEND DEP_FIND_CODE "find_dependency(tinyxml2 REQUIRED)\n")
message(STATUS "Generated DEP_FIND_CODE:\n${DEP_FIND_CODE}")

# 7. 定义库并关联代码
#file(GLOB_RECURSE SOURCES "src/*.cpp")
if (BUILD_SHARED_LIBS)
    #适合没有子模块的库
    #    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    #适合有子模块的库
    add_library(${PROJECT_NAME} SHARED)
else ()
    #    add_library(${PROJECT_NAME} STATIC ${SOURCES})
    #适合有子模块的库
    add_library(${PROJECT_NAME} STATIC
            include/common-utils/Export.h)
endif ()

# 添加子模块(适合有子模块的库)
add_subdirectory(src/core)
add_subdirectory(src/xlsx)
add_subdirectory(src/xml)
add_subdirectory(src/log)

# 设置模块源码(适合有子模块的库)
target_sources(${PROJECT_NAME} PRIVATE
        $<TARGET_OBJECTS:common-utils-core>
        $<TARGET_OBJECTS:common-utils-xlsx>
        $<TARGET_OBJECTS:common-utils-xml>
        $<TARGET_OBJECTS:common-utils-log>
)

# 8. 是否打开测试用例
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(BUILD_GOOGL_TESTS "Build unit tests" ON)
    option(BUILD_MY_TESTS "Build unit tests" ON)
endif ()

# ===================================== 以下无需修改 =====================================
# ========== 3. 生成跨平台的导出头文件（仅动态库需要） ==========
if (BUILD_SHARED_LIBS)
    include(GenerateExportHeader)
    set(EXPORT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/${LIB_NAME}/${LIB_NAME}Export.h")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/${LIB_NAME}")

    # 将连字符替换为下划线
    string(REPLACE "-" "_" LIB_NAME_UNDERSCORE ${LIB_NAME})  # common_utils
    string(TOUPPER "${LIB_NAME_UNDERSCORE}" UPPER_LIB_NAME)  # COMMON_UTILS
    generate_export_header(${PROJECT_NAME}
            EXPORT_MACRO_NAME ${UPPER_LIB_NAME}_EXPORT
            EXPORT_FILE_NAME ${EXPORT_HEADER}
    )
    message(STATUS "Export header generated at: ${EXPORT_HEADER}")
    # 动态库导出符号控制
    target_compile_definitions(${PROJECT_NAME} PUBLIC
            ${LIB_NAME_UNDERSCORE}_EXPORTS>
    )
endif ()

# ========== 4. 链接依赖库 + 静态模式强制 -static ==========
if (DEP_TARGETS)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_TARGETS})
endif ()

# ========== 5. 设置包含目录 ==========
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ========== 6. 设置目标属性 ==========
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        WINDOWS_EXPORT_ALL_SYMBOLS $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,ON,OFF>
)

# ========== 7. 安装bin/lib以及头文件 ==========
include(GNUInstallDirs)
#安装bin,lib
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 显式安装头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIB_NAME}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}
        FILES_MATCHING PATTERN "*.h"
        PATTERN "*.hpp"
)
#安装导出头文件
if (BUILD_SHARED_LIBS)
    install(FILES ${EXPORT_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME})
endif ()

# 8. 生成find_package配置文件Config.cmake，ConfigVersion.cmake以及Targets.cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
        #        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake.base  # 使用临时文件
        INSTALL_DESTINATION share/${PROJECT_NAME}
)
# 读取基础文件Config.cmake，并将依赖写进去
file(READ ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake.base BASE_CONTENT)
# 在 include(CMakeFindDependencyMacro) 之后插入依赖代码
string(REPLACE
        "include(CMakeFindDependencyMacro)"
        "include(CMakeFindDependencyMacro)\n${DEP_FIND_CODE}"
        FINAL_CONTENT "${BASE_CONTENT}"
)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake "${FINAL_CONTENT}")

#ConfigVersion.cmake
write_basic_package_version_file(
        ${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)
#安装Config.cmake以及ConfigVersion.cmake
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION share/${PROJECT_NAME}
)
#安装Targets.cmake
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION share/${PROJECT_NAME}
)

# ========== 9. MSVC运行时库配置 ==========
if (MSVC)
    if (BUILD_SHARED_LIBS)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    else ()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "MSVC runtime for static lib")
    endif ()
endif ()

# ========== 10. 单元测试配置（可选） ==========
if (BUILD_GOOGL_TESTS)
    message(STATUS "Building unit tests...")
    add_subdirectory(tests/gtest)
endif ()

if (BUILD_MY_TESTS)
    message(STATUS "Building unit tests...")
    add_subdirectory(tests/test)
endif ()

# ===================================== 使用说明 =====================================
# 1. 在库源码根目录创建 cmake/ 文件夹，并放置 ${PROJECT_NAME}Config.cmake.in 文件
#    模板内容：
#    @PACKAGE_INIT@
#
#    include(CMakeFindDependencyMacro)
#    @PACKAGE_DEPENDENCIES@
#
#    include("${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@Targets.cmake")
#    check_required_components(@PROJECT_NAME@)
#
# 2. 确保 src/ 和 include/ 目录存在，且头文件放在 include/${LIB_NAME}/ 下
#