set(EXE_NAME ${PROJECT_NAME}-tcp-file)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
qt_add_executable(${EXE_NAME}
        MANUAL_FINALIZATION
        tcpfiletransferclient.cpp
        tcpfiletransferserver.cpp
        tcpfiletransferclient.ui
        tcpfiletransferserver.ui
        main.cpp
)

# h,cpp以及ui分类存放时才需要

target_link_libraries(${EXE_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
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
