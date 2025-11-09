project(CmakeTutorials.Tutorial3 VERSION 1.0)

# add the executable
add_executable(${PROJECT_NAME} tutorial.cxx)

# add the MathFunctions library
add_subdirectory(MathFunctions)

target_link_libraries(${PROJECT_NAME} PRIVATE Tutorial3.MathFunctions)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_BINARY_DIR}"
        #    ${LIB_HEADER_INCLUDES} 这里不需要包含MathFunctions library header的原因是：
        #    在子目录的cmakelist中为MathFunctions添加头文件时使用了PUBLIC，会自动递给链接到 MathFunctions 的目标
        #    target_include_directories(MathFunctions PUBLIC ...)
)
