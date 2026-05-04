project(dynamic_pdp_discovery VERSION 1.0.0 LANGUAGES CXX)

# Set CMAKE_BUILD_TYPE to Release by default.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Configuring discovery server example...")
file(GLOB DISCOVERY_SERVER_SOURCES_CXX "*.cxx")
file(GLOB DISCOVERY_SERVER_SOURCES_CPP "*.cpp")

add_executable(${PROJECT_NAME} ${DISCOVERY_SERVER_SOURCES_CXX} ${DISCOVERY_SERVER_SOURCES_CPP})
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    $<$<BOOL:${SHM_TRANSPORT_DEFAULT}>:SHM_TRANSPORT_BUILTIN> # Enable SHM as built-in transport
)
target_link_libraries(${PROJECT_NAME} fastdds fastcdr)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${DATA_INSTALL_DIR}/fastdds/examples/${PROJECT_NAME}/${BIN_INSTALL_DIR})

