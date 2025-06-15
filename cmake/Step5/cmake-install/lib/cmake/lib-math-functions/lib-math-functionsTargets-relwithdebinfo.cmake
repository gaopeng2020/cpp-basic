#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "MathFunctions" for configuration "RelWithDebInfo"
set_property(TARGET MathFunctions APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(MathFunctions PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libMathFunctions.a"
  )

list(APPEND _cmake_import_check_targets MathFunctions )
list(APPEND _cmake_import_check_files_for_MathFunctions "${_IMPORT_PREFIX}/lib/libMathFunctions.a" )

# Import target "SqrtLibrary" for configuration "RelWithDebInfo"
set_property(TARGET SqrtLibrary APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(SqrtLibrary PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libSqrtLibrary.a"
  )

list(APPEND _cmake_import_check_targets SqrtLibrary )
list(APPEND _cmake_import_check_files_for_SqrtLibrary "${_IMPORT_PREFIX}/lib/libSqrtLibrary.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
