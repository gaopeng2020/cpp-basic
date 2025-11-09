set(LIB_PREFIX Calculator) # 通用模板，只需修改这里

set(LIB_TYPE SHARED CACHE STRING "库类型（SHARED动态库/STATIC静态库）") # 默认生成动态库
#=====================================以下无需修改==================================================
cmake_minimum_required(VERSION 3.15)
project(${LIB_PREFIX}Lib VERSION 1.0.0 LANGUAGES CXX)

# 0. 控制动态/静态库的变量（新增）
set_property(CACHE LIB_TYPE PROPERTY STRINGS SHARED STATIC) # 限定输入值类型
if(NOT LIB_TYPE STREQUAL "SHARED" AND NOT LIB_TYPE STREQUAL "STATIC")
    message(FATAL_ERROR "LIB_TYPE must be SHARED or STATIC")
endif()

# 1. 基本配置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install" CACHE PATH "Installation directory")

# 2. 源文件设置
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# 3. 创建目标（根据LIB_TYPE变量切换库类型）
add_library(${PROJECT_NAME} ${LIB_TYPE} ${SOURCES})

# 4. 生成跨平台的导出头文件（仅动态库需要导出宏）
if(LIB_TYPE STREQUAL "SHARED")
    include(GenerateExportHeader)
    set(EXPORT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/${LIB_PREFIX}/${LIB_PREFIX}Export.h")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/${LIB_PREFIX}")
    string(TOUPPER "${LIB_PREFIX}" UPPER_LIB_PREFIX)
    generate_export_header(${PROJECT_NAME}
            EXPORT_MACRO_NAME ${UPPER_LIB_PREFIX}_EXPORT
            EXPORT_FILE_NAME ${EXPORT_HEADER}
    )
    # 将导出头文件加入头文件列表（仅在动态库时安装）
    list(APPEND HEADERS ${EXPORT_HEADER})
endif()

# 5. 包含目录设置
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 6. 条件链接数学库（Windows无需链接）
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC m)
endif()

# 7. 版本和属性配置
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER "${HEADERS}"
        # 动态库自动处理符号导出，静态库无需此设置
        WINDOWS_EXPORT_ALL_SYMBOLS $<IF:$<STREQUAL:${LIB_TYPE},SHARED>,ON,OFF>
)

# 8. 安装配置
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}        # DLL安装到bin目录
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}       # 类Unix的.so/.dylib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}       # 静态库和导入库（如MSVC的.lib）
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_PREFIX}
)

# 仅动态库安装导出头文件
if(LIB_TYPE STREQUAL "SHARED")
    install(FILES ${EXPORT_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_PREFIX})
endif()

# 导出目标文件
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# 9. 生成配置文件
include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
        ${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

# 安装配置文件
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# 可选：MSVC运行时库配置（动态库使用MD，静态库可自行选择）
if(MSVC)
    # 动态库强制使用动态运行时库（MD/MDd）
    if(LIB_TYPE STREQUAL "SHARED")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    else()
        # 静态库默认使用静态运行时库（MT/MTd），但允许用户覆盖
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "MSVC runtime for static lib")
    endif()
endif()

# 在调用动态链接库时，需要将dll复制到项目的编译目录，可以通过如下命令自动复制（该命令仅在调用出的CMakeList中使用）
if (WIN32)
    # 直接通过目标属性获取动态库位置
    get_target_property(DLL_PATH CalculatorLib::CalculatorLib IMPORTED_LOCATION_RELEASE)
    if (NOT DLL_PATH)
        get_target_property(DLL_PATH CalculatorLib::CalculatorLib IMPORTED_LOCATION)
    endif ()
    message(STATUS "DLL_PATH = " ${DLL_PATH})

    # 执行复制
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${DLL_PATH}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying CalculatorLib.dll to output directory"
    )
endif ()