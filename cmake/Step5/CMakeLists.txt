# build and install
# cd build
# cmake ..
# make 或者 cmake --build . --config Release
# cmake --install . --prefix "../install" --config Release

cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Tutorial VERSION 1.0)

# set(EXECUTABLE_OUTPUT_PATH ./bin) 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 获取编译时间戳
string(TIMESTAMP COMPILE_TIME "%Y-%m-%d %H:%M:%S")
set(BUILD_TIME    ${COMPILE_TIME})

include(GNUInstallDirs)

# 添加自定义cmake文件目录到CMAKE_MODULE_PATH
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
# 引用Utility.cmake
include(Utility)

#git commit号
set(GIT_HASH "")
get_git_hash(GIT_HASH)
#git分支
set(GIT_BRANCH "")
get_git_branch(GIT_BRANCH)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# 复制一份输入文件到输出文件，替换输入文件中被@VAR@或者${VAR}引用的变量值
# configure_file(<input> <output>
#                [ESCAPE_QUOTES] [@ONLY]
#                [NEWLINE_STYLE [UNIX|DOS|WIN32|LF|CRLF] ])
configure_file(TutorialConfig.h.in TutorialConfig.h @ONLY)

# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(${PROJECT_NAME} tutorial.cxx)

# add compile options
target_compile_options(${PROJECT_NAME} INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

# link libraries
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC MathFunctions)
# target_link_directories(${PROJECT_NAME} PUBLIC lib——dir)

# TODO 3: Install Tutorial in the bin directory
# Hint: Use the TARGETS and DESTINATION parameters
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# TODO 4: Install TutorialConfig.h to the include directory
# Hint: Use the FILES and DESTINATION parameters
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
