set (PACKAGE_NAME "lib-math-functions")
set (LIBRARY_INCLUDE_DIRS include)
set (LIBRARY_INSTALL_DIR lib)
set (LIBRARY_NAME "MathFunctions")
set (CAMKE_FILES_INSTALL_DIR ${LIBRARY_INSTALL_DIR}/cmake/${PACKAGE_NAME})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_library(${LIBRARY_NAME} STATIC MathFunctions.cxx)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(${LIBRARY_NAME}
                           INTERFACE 
                          #  ${CMAKE_CURRENT_SOURCE_DIR}
                          $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # Export Configuration for find_package
                          $<INSTALL_INTERFACE:include> # Export Configuration for find_package
                          )

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
  target_compile_definitions(${LIBRARY_NAME} PRIVATE "USE_MYMATH")

  # library that just does sqrt
  add_library(SqrtLibrary STATIC mysqrt.cxx)

  target_link_libraries(${LIBRARY_NAME} PRIVATE SqrtLibrary)
endif()


# ------------------------------------install---------------------------------------
# TODO 1: Create a variable called installable_libs that is a list of all
# libraries we want to install (e.g. MathFunctions and tutorial_compiler_flags)
# Then install the installable libraries to the lib folder.
# Hint: Use the TARGETS and DESTINATION parameters
set(installable_libs ${LIBRARY_NAME} ) #tutorial_compiler_flags)
if(TARGET SqrtLibrary)
  list(APPEND installable_libs SqrtLibrary)
endif()

install(TARGETS ${installable_libs}
        EXPORT ${PACKAGE_NAME}Targets # Export Configuration for find_package
        DESTINATION ${LIBRARY_INSTALL_DIR}
)

# TODO 2: Install the library headers to the include folder.
# Hint: Use the FILES and DESTINATION parameters
install(FILES MathFunctions.h DESTINATION ${LIBRARY_INCLUDE_DIRS})


# -------------------------Export Configuration for find_package---------------------
include(CMakePackageConfigHelpers)

# generate the config file that includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake"
    INSTALL_DESTINATION ${CAMKE_FILES_INSTALL_DIR}
    PATH_VARS LIBRARY_INCLUDE_DIRS
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake
    VERSION 1.2.3
    COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake
  DESTINATION ${CAMKE_FILES_INSTALL_DIR}
)

install(EXPORT ${PACKAGE_NAME}Targets
  FILE ${PACKAGE_NAME}Targets.cmake
  DESTINATION ${CAMKE_FILES_INSTALL_DIR}
)

export(EXPORT ${PACKAGE_NAME}Targets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Targets.cmake"
)