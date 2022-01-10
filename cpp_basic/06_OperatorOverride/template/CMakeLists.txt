set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/lib)
include_directories(${CMAKE_SOURCE_DIR}/include)
aux_source_directory(./ SRCS)
add_library(OperatorOverride ${SRCS})