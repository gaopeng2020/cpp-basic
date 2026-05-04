project(fastdds_xtypes_example VERSION 1 LANGUAGES CXX)

# Set CMAKE_BUILD_TYPE to Release by default.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Configuring ${PROJECT_NAME}...")
file(GLOB EXAMPLE_SOURCES "*.cpp")

add_executable(xtypes ${EXAMPLE_SOURCES})
target_compile_definitions(xtypes PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    $<$<BOOL:${SHM_TRANSPORT_DEFAULT}>:SHM_TRANSPORT_BUILTIN> # Enable SHM as built-in transport
)
target_link_libraries(xtypes fastdds fastcdr)
install(TARGETS xtypes
    RUNTIME DESTINATION ${DATA_INSTALL_DIR}/fastdds/examples/cpp/xtypes/${BIN_INSTALL_DIR})

# Copy the XML files over to the build directory
file(GLOB_RECURSE XML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.xml)
# for each xml file detected
foreach(XML_FILE_COMPLETE_PATH ${XML_FILES})
    # obtain the file name
    get_filename_component(XML_FILE ${XML_FILE_COMPLETE_PATH} NAME_WE)
    # copy the file from src to build folders
    configure_file(
        ${XML_FILE_COMPLETE_PATH}                   # from full src path
        ${CMAKE_CURRENT_BINARY_DIR}/${XML_FILE}.xml # to relative build path
        COPYONLY)
    install(FILES ${XML_FILE_COMPLETE_PATH}
        DESTINATION ${DATA_INSTALL_DIR}/fastdds/examples/cpp/xtypes/${BIN_INSTALL_DIR})
endforeach()
