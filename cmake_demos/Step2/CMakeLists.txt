project(CmakeTutorials.Tutorial2 VERSION 1.0)

# Customized variable
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# add the executable
add_executable(${PROJECT_NAME} tutorial.cxx)

# link the library to our executable
if (USE_MYMATH)
    add_subdirectory(MathFunctions)
    list(APPEND EXTRA_LIBS Tutorial2.MyMathFunctions)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")

    # link the library to our executable
    target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTRA_LIBS})

    # Add library‘s header to our executable
    target_include_directories(${PROJECT_NAME} PRIVATE
            "${EXTRA_INCLUDES}"
    )
endif ()

# Add version configuration header to our executable
target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_BINARY_DIR}"
)


                          