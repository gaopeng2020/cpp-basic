cmake_minimum_required(VERSION 3.0.0)
project(01_Pointer VERSION 0.1.0 LANGUAGES C)

# include(CTest)
# enable_testing()

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCS)
add_executable(${PROJECT_NAME} ${SRCS})

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
