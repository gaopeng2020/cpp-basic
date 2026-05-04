project(CmakeTutorials.Tutorial1 VERSION 1.5.0)

add_executable(${PROJECT_NAME} tutorial.cxx)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_BINARY_DIR}")
