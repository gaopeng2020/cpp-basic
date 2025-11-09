add_library(Tutorial7.MathFunctions MathFunctions.cxx)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(Tutorial7.MathFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
    target_compile_definitions(Tutorial7.MathFunctions PRIVATE "USE_MYMATH")

    # library that just does sqrt
    add_library(Tutorial7.SqrtLibrary STATIC mysqrt.cxx)

    # TODO 1: Include CheckCXXSourceCompiles
    include(CheckCXXSourceCompiles)

    # TODO 2: Use check_cxx_source_compiles with simple C++ code to verify
    # availability of:
    # * std::log
    # * std::exp
    # Store the results in HAVE_LOG and HAVE_EXP respectively.

    # Hint: Sample C++ code which uses log:
    # #include <cmath>
    # int main() {
    #   std::log(1.0);
    #   return 0;
    # }
    check_cxx_source_compiles("
  #include <cmath>
  int main() {
    std::log(1.0);
    return 0;
  }
" HAVE_LOG)

    check_cxx_source_compiles("
  #include <cmath>
  int main() {
    std::exp(1.0);
    return 0;
  }
" HAVE_EXP)

    # TODO 3: Conditionally on HAVE_LOG and HAVE_EXP, add private compile
    # definitions "HAVE_LOG" and "HAVE_EXP" to the SqrtLibrary target.

    # Hint: Use target_compile_definitions()
    if (HAVE_LOG AND HAVE_EXP)
        target_compile_definitions(Tutorial7.MathFunctions PRIVATE "HAVE_LOG" "HAVE_EXP")
    endif ()

    target_link_libraries(Tutorial7.MathFunctions PRIVATE Tutorial7.SqrtLibrary)
endif ()


# install libs
#set(installable_libs Tutorial7.MathFunctions tutorial_compiler_flags)
if (TARGET Tutorial7.SqrtLibrary)
    list(APPEND installable_libs Tutorial7.SqrtLibrary)
endif ()
install(TARGETS ${installable_libs} DESTINATION lib)
# install include headers
install(FILES MathFunctions.h DESTINATION include)
