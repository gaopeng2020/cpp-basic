add_executable(${PROJECT_NAME}-file-client
        FileTransportSession.cpp
        FileTransportClient.cpp
)
target_link_libraries(${PROJECT_NAME}-file-client PRIVATE asio::asio)
if (WIN32)
    target_compile_definitions(${PROJECT_NAME}-file-client PRIVATE _WIN32_WINNT=0x0A00)
endif ()

# --------------------------------------------server------------------------------------------------
add_executable(${PROJECT_NAME}-file-server
        FileTransportSession.cpp
        FileTransportServer.cpp
)
target_link_libraries(${PROJECT_NAME}-file-server PRIVATE asio::asio)
if (WIN32)
    target_compile_definitions(${PROJECT_NAME}-file-server PRIVATE _WIN32_WINNT=0x0A00)
endif ()


