# 使用绝对路径
#set(SOURCE_PATH "D:\\Documents\\gitlab\\cpp-basic\\conmmon-utils")
get_filename_component(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

# 配置 CMake
vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
        OPTIONS
        -DBUILD_SHARED_LIBS=ON
        -DCMAKE_INSTALL_INCLUDEDIR=include
        -DCMAKE_INSTALL_LIBDIR=lib
        -DCMAKE_INSTALL_BINDIR=bin
        -DBUILD_GOOGL_TESTS=OFF
        -DBUILD_MY_TESTS=OFF
)

# 构建和安装
#vcpkg_install_cmake()
# 构建
vcpkg_cmake_build()

# 安装到 staging 目录
vcpkg_cmake_install()

# 如果 CMakeLists.txt 里写了 install(EXPORT ...)，这行会自动生成正确的 *Config.cmake
vcpkg_cmake_config_fixup(
        PACKAGE_NAME common-utils
        CONFIG_PATH share/common-utils
)

# 清理调试目录的重复文件
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

# 复制 PDB 文件
vcpkg_copy_pdbs()

# 安装 usage 文件
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")

# 处理版权文件
if (NOT EXISTS "${SOURCE_PATH}/LICENSE")
    file(WRITE "${CURRENT_PACKAGES_DIR}/share/${PORT}/copyright" "MIT License\n")
else ()
    file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
endif ()
