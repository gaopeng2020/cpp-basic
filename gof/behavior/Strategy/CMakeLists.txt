cmake_minimum_required(VERSION 3.10)
project(StrategyPattern LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(StrategyDemo
        main.cpp
        TaxStategy.cpp
        ShangHaiTaxStategy.cpp
)

target_include_directories(StrategyDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
