set(CLIENT_NAME "${PROJECT_NAME}-udp-daytime-client")
add_executable(${CLIENT_NAME} udp_daytime_client.cpp)
target_link_libraries(${CLIENT_NAME} PRIVATE asio::asio)
if (WIN32)
    target_compile_definitions(${CLIENT_NAME} PRIVATE _WIN32_WINNT=0x0A00)
endif ()

# --------------------------------------------server------------------------------------------------
set(SERVER_NAME "${PROJECT_NAME}-udp-daytime-server")
add_executable(${SERVER_NAME} udp_daytime_server.cpp)
target_link_libraries(${SERVER_NAME} PRIVATE asio::asio)
if (WIN32)
    target_compile_definitions(${SERVER_NAME} PRIVATE _WIN32_WINNT=0x0A00)
endif ()


