# set the project name and version
project(CmakeTutorials.Tutorial7 VERSION 1.0)

# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(${PROJECT_NAME} tutorial.cxx)

target_link_libraries(${PROJECT_NAME} PUBLIC Tutorial7.MathFunctions)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}")

# add the install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h" DESTINATION include)

# enable testing
include(CTest)

# does the application run
add_test(NAME Test1 COMMAND CmakeTutorials.Tutorial7 25)

# does the usage message work?
add_test(NAME Test2 COMMAND CmakeTutorials.Tutorial7)
set_tests_properties(Test2 PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number")

# define a function to simplify adding tests
function(do_test target arg result)
    add_test(NAME Test${arg} COMMAND ${target} ${arg})
    set_tests_properties(Test${arg} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endfunction()

# do a bunch of result based tests
# do_test(CmakeTutorials.Tutorial7 4 "4 is 2")
do_test(CmakeTutorials.Tutorial7 9 "9 is 3")
# do_test(CmakeTutorials.Tutorial7 5 "5 is 2.236")
# do_test(CmakeTutorials.Tutorial7 7 "7 is 2.645")
# do_test(CmakeTutorials.Tutorial7 25 "25 is 5")
do_test(CmakeTutorials.Tutorial7 -25 "-25 is (-nan|nan|0)")
do_test(CmakeTutorials.Tutorial7 0.0001 "0.0001 is 0.01")
