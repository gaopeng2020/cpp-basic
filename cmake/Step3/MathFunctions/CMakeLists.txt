add_library(MathFunctions MathFunctions.cxx)

# TODO 1: State that anybody linking to MathFunctions needs to include the
# current source directory, while MathFunctions itself doesn't.
# Hint: Use target_include_directories with the INTERFACE keyword

# library 嵌套：MathFunctions嵌套MySqrtLibrary
set(LibName MySqrtLibrary)
# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
# 将USE_MYMATH变量传递给MathFunctions
  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")

  # library that just does sqrt
  add_library(${LibName} STATIC mysqrt.cxx)

  # TODO 7: Link SqrtLibrary to MathFunctions
  target_link_libraries(MathFunctions PUBLIC ${LibName})
endif()

# TODO 6: Link include for MathFunctions
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )