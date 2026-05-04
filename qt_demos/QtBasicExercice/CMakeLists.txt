#ui不在根目录时必须告诉cmake ui的路径
set(CMAKE_AUTOUIC_SEARCH_PATHS "ui")
find_package(common-utils CONFIG REQUIRED)

set(EXE_NAME AdtConsistencyChecker)

set(PROJECT_SOURCES
        src/main.cpp
        src/AdtConsistencyCheckApp.cpp
        src/DataTypeConsistencyCheck.cpp
        # .h,cpp以及ui分类存放时，必须添加头文件以及ui，否则找不到ui_xxx.h
        include/AdtConsistencyCheckApp.h
        include/DataTypeConsistencyCheck.h
        ui/AdtConsistencyCheckApp.ui
)

qt_add_executable(${EXE_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resource.qrc
        resource.rc
)

# h,cpp以及ui分类存放时才需要
target_include_directories(${EXE_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        common-utils::common-utils
)

# 构建时复制conf文件到CMAKE_BINARY_DIR
add_custom_command(
        TARGET ${EXE_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/conf
        ${CMAKE_BINARY_DIR}/conf
        COMMENT "Copying config directory to build folder"
)

set_target_properties(${EXE_NAME} PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${EXE_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${EXE_NAME})
endif ()
