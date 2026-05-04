add_library(Tutorial8.MathFunctions MathFunctions.cxx)

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
    target_compile_definitions(Tutorial8.MathFunctions PRIVATE "USE_MYMATH")

    # library that just does sqrt
    add_library(Tutorial8.SqrtLibrary STATIC mysqrt.cxx)

    # does this system provide the log and exp functions?
    include(CheckCXXSourceCompiles)
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

    # add compile definitions
    if (HAVE_LOG AND HAVE_EXP)
        target_compile_definitions(Tutorial8.SqrtLibrary
                PRIVATE "HAVE_LOG" "HAVE_EXP"
        )
    endif ()

    target_link_libraries(Tutorial8.MathFunctions PRIVATE Tutorial8.SqrtLibrary)
endif ()

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(Tutorial8.MathFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


# install libs
if (TARGET Tutorial8.SqrtLibrary)
    list(APPEND installable_libs Tutorial8.SqrtLibrary)
endif ()
install(TARGETS ${installable_libs} DESTINATION lib)
# install include headers
install(FILES MathFunctions.h DESTINATION include)
