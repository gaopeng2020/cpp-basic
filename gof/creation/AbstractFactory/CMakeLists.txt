cmake_minimum_required(VERSION 3.10)
project(AbstractFactoryPattern LANGUAGES CXX)

file(GLOB SOURCES "src/*.cpp")

add_executable(AbstractFactoryDemo
        main.cpp
        ${SOURCES}
)

target_include_directories(AbstractFactoryDemo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
