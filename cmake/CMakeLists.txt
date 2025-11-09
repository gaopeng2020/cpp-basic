cmake_minimum_required(VERSION 3.15)
project(CmakeTutorials VERSION 1.0.0)

#定义C++版本
#add_definitions(-std=c++20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(CmakeTutorialConfig.h.in TutorialConfig.h)

#控制所有运行时文件（可执行文件、DLL 等）的输出目录。
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#设置Dynamic lib文件存放位置
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#设置Static lib文件存放位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_INSTALL_PREFIX ../install)

add_subdirectory(Step1)
add_subdirectory(Step2)
add_subdirectory(Step3)
add_subdirectory(Step4)
add_subdirectory(Step5)
add_subdirectory(Step6)
add_subdirectory(Step7)
add_subdirectory(Step8)
#add_subdirectory(Step9)
#add_subdirectory(Step10)
#add_subdirectory(Step11)
#add_subdirectory(Step12)