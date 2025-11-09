add_library(Tutorial3.MathFunctions MathFunctions.cxx)

# library 嵌套：MathFunctions嵌套MySqrtLibrary
# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
    # 将USE_MYMATH变量传递给MathFunctions
    target_compile_definitions(Tutorial3.MathFunctions PRIVATE "USE_MYMATH")

    set(LibName Tutorial3.MySqrtLibrary)
    # library that just does sqrt
    add_library(${LibName} STATIC mysqrt.cxx)

    # TODO 7: Link SqrtLibrary to MathFunctions
    target_link_libraries(Tutorial3.MathFunctions PUBLIC ${LibName})
endif ()

#  include for MathFunctions
# PRIVATE：仅对当前目标可见。
# PUBLIC：对当前目标及其链接的目标可见。
# INTERFACE：仅对链接到当前目标的目标可见（通常用于库）。
target_include_directories(Tutorial3.MathFunctions PUBLIC $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}>)