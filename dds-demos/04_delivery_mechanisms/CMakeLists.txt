project(fastdds_delivery_mechanisms_example VERSION 1 LANGUAGES CXX)

# Set CMAKE_BUILD_TYPE to Release by default.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Configuring delivery mechanisms example...")
file(GLOB DELIVERY_MECHANISMS_SOURCES_CXX "*.cxx")
file(GLOB DELIVERY_MECHANISMS_SOURCES_CPP "*.cpp")

add_executable(delivery_mechanisms
    ${DELIVERY_MECHANISMS_SOURCES_CXX}
    ${DELIVERY_MECHANISMS_SOURCES_CPP})

target_link_libraries(delivery_mechanisms fastdds fastcdr)

install(TARGETS delivery_mechanisms
    RUNTIME DESTINATION ${DATA_INSTALL_DIR}/fastdds/examples/cpp/delivery_mechanisms/${BIN_INSTALL_DIR})
