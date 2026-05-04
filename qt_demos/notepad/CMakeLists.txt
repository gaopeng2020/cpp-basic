#cmake_minimum_required(VERSION 3.5)

#project(notepad VERSION 0.1 LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5
        REQUIRED COMPONENTS Widgets LinguistTools
        OPTIONAL_COMPONENTS PrintSupport
)
find_package(Qt${QT_VERSION_MAJOR}
        REQUIRED COMPONENTS Widgets LinguistTools
        OPTIONAL_COMPONENTS PrintSupport
)

set(EXE_NAME ${PROJECT_NAME}-notepad)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        finddialog.h
        finddialog.cpp
        notepad_zh_CN.ts
        resources.qrc
        findreplacedialog.ui
)

qt_add_executable(${EXE_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
)
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} notepad_zh_CN.ts)


target_link_libraries(${EXE_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
if (TARGET Qt6::PrintSupport)
    target_link_libraries(${EXE_NAME} PRIVATE Qt6::PrintSupport)
endif ()

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
