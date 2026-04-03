    enable_testing()
    find_package(. CONFIG REQUIRED)

    # 收集所有测试源文件
    file(GLOB_RECURSE TEST_SOURCES "*.cpp")
    add_executable(${PROJECT_NAME}_gtest ${TEST_SOURCES})

    target_link_libraries(${PROJECT_NAME}_gtest PRIVATE
            GTest::gtest
            GTest::gtest_main
            ${PROJECT_NAME}
    )
    # 设置包含目录
    target_include_directories(${PROJECT_NAME}_gtest PRIVATE
            ..
            ${CMAKE_SOURCE_DIR}/include
    )

    # 注册测试
    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_gtest
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            PROPERTIES VS_DTE_START_PATH "${CMAKE_BINARY_DIR}/$<TARGET_FILE_DIR:${PROJECT_NAME}_gtest>"
    )
