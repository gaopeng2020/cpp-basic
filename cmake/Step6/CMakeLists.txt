cmake_minimum_required(VERSION 3.31)
project(CmakeTutorials.Tutorial6)

set(CMAKE_CXX_STANDARD 20)

# 设置查找路径
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/../install")
find_package(CalculatorLib REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE CalculatorLib::CalculatorLib)

# Windows下需要自动复制DLL到可执行文件目录,否则运行时找不到动态链接库
if (WIN32)
    # 直接通过目标属性获取动态库位置 如果库为Release则需要修改为IMPORTED_LOCATION_RELEASE
    get_target_property(DLL_PATH CalculatorLib::CalculatorLib IMPORTED_LOCATION_DEBUG)
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
