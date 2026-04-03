# 使用绝对路径
set(SOURCE_PATH "D:\\Documents\\gitlab\\cpp-basic\\conmmon-utils")

# 配置 CMake
vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
        OPTIONS
        -DLIB_TYPE=SHARED
        -DCMAKE_INSTALL_INCLUDEDIR=include
        -DCMAKE_INSTALL_LIBDIR=lib
        -DCMAKE_INSTALL_BINDIR=bin
)

# 构建和安装
#vcpkg_install_cmake()
# 构建
vcpkg_cmake_build()

# 安装到 staging 目录
vcpkg_cmake_install()

# 清理调试目录的重复文件
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

# 复制 PDB 文件
vcpkg_copy_pdbs()

# 安装 usage 文件
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")

# 处理版权文件
if(NOT EXISTS "${SOURCE_PATH}/LICENSE")
    file(WRITE "${CURRENT_PACKAGES_DIR}/share/${PORT}/copyright" "MIT License\n")
else()
    file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
endif()
