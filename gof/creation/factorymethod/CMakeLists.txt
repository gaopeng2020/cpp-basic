cmake_minimum_required(VERSION 3.10)
project(FactoryMethodPattern LANGUAGES CXX)

file(GLOB SOURCES "src/*.cpp")

add_executable(FactoryMethodDemo
        main.cpp
        ${SOURCES}
)

target_include_directories(FactoryMethodDemo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
