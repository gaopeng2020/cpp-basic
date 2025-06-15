set(PACKAGE_NAME lib-math-functions)

get_filename_component(${PACKAGE_NAME}_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(${PACKAGE_NAME}_INCLUDE_DIRS "include")
set(${PACKAGE_NAME}_LIBRARIES "MathFunctions")


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was Config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

####################################################################################

include ("${CMAKE_CURRENT_LIST_DIR}/${PACKAGE_NAME}Targets.cmake")
