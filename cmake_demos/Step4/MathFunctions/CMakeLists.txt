# create the MathFunctions library
add_library(Tutorial4.MathFunctions STATIC MathFunctions.cxx)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(Tutorial4.MathFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
    #  为指定的目标添加预处理器宏定义，使用PRIVATE USE_MYMATH 只对 MathFunctions 的编译过程有效，不会影响链接到MathFunctions的Target编译过程
    target_compile_definitions(Tutorial4.MathFunctions PRIVATE "USE_MYMATH")

    # library that just does sqrt
    add_library(Tutorial4.SqrtLibrary STATIC mysqrt.cxx)

    # link our compiler flags interface library
    target_link_libraries(Tutorial4.SqrtLibrary PUBLIC tutorial_compiler_flags)
    target_link_libraries(Tutorial4.MathFunctions PUBLIC Tutorial4.SqrtLibrary)
endif ()

# link our compiler flags interface library
target_link_libraries(Tutorial4.MathFunctions PUBLIC tutorial_compiler_flags)
