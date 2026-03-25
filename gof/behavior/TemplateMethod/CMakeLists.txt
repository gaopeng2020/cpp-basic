cmake_minimum_required(VERSION 3.10)
project(TemplateMethodPattern LANGUAGES CXX)

add_executable(TemplateMethodDemo
        main.cpp
        AppClass.cpp
        LibClass.cpp
)

target_include_directories(TemplateMethodDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
