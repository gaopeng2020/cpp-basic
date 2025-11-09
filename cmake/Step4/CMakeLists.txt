# set the project name and version
project(CmakeTutorials.Tutorial4 VERSION 1.0)

# specify the C++ standard
add_library(tutorial_compiler_flags INTERFACE)

# 为目标指定所需的 C++ 标准特性,确保编译器在编译目标时启用所需的 C++ 特性
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_17)
# Add warning flag compile options to the interface library
# tutorial_compiler_flags.
# * For gcc_like_cxx, add flags -Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused
# * For msvc_cxx, add flags -W3
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# Hint: Use target_compile_options()
target_compile_options(tutorial_compiler_flags INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(${PROJECT_NAME} tutorial.cxx)

target_link_libraries(${PROJECT_NAME} PUBLIC Tutorial4.MathFunctions tutorial_compiler_flags)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}")
