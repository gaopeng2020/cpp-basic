cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Release")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# specify search path for find_package
set (PACKAGE_NAME "lib-math-functions")
set(${PACKAGE_NAME}_DIR ${PROJECT_SOURCE_DIR}/../Step5/build/MathFunctions/)
set(${PACKAGE_NAME}_DIR ${PROJECT_SOURCE_DIR}/../Step5/install/lib/cmake/${PACKAGE_NAME}/) 
find_package(${PACKAGE_NAME} REQUIRED)
# message ( STATUS "${PACKAGE_NAME}_LIBRARIES = ${${PACKAGE_NAME}_LIBRARIES}" )
# message ( STATUS "${PACKAGE_NAME}_INCLUDE_DIRS = ${${PACKAGE_NAME}_INCLUDE_DIRS}")

# add the executable
add_executable(${PROJECT_NAME} tutorial.cxx)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${PROJECT_BINARY_DIR}
    ${${PACKAGE_NAME}_INCLUDE_DIR}
)

# add link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${${PACKAGE_NAME}_LIBRARIES})
# target_link_directories(${PROJECT_NAME} PRIVATE ${${PACKAGE_NAME}_INCLUDE_DIR})
