cmake_minimum_required(VERSION 3.20)

# Windows 平台
if (WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "vcpkg triplet")
elseif (APPLE)
    # macOS 平台
    set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "vcpkg triplet")
elseif (UNIX)
    # Linux 平台
    set(VCPKG_TARGET_TRIPLET "x64-linux-dynamic" CACHE STRING "vcpkg triplet")
endif ()
message(STATUS "Auto-detected vcpkg triplet: ${VCPKG_TARGET_TRIPLET}")

if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain file")
    set(CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}")
endif ()


project(c_cpp_basic VERSION 1.0.0)

#定义C++版本
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add_subdirectory(c_basic)
#add_subdirectory(cpp_basic)
#add_subdirectory(cmake_demos)
#add_subdirectory(common-utils)
#add_subdirectory(qt_demos)
add_subdirectory(dds-demos)
#add_subdirectory(asio_demos)
#add_subdirectory(crypto)

