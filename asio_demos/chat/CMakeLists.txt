add_executable(${PROJECT_NAME}-chat-client
        chat_message.hpp
        chat_client.cpp
)
target_link_libraries(${PROJECT_NAME}-chat-client PRIVATE asio::asio)
# 添加这一行来定义 Windows 目标版本
# 0x0601: Windows 7
# 0x0602: Windows 8
# 0x0603: Windows 8.1
# 0x0A00: Windows 10 / 11 (推荐)
if (WIN32)
    target_compile_definitions(${PROJECT_NAME}-chat-client PRIVATE _WIN32_WINNT=0x0A00)
endif ()

# --------------------------------------------server------------------------------------------------
add_executable(${PROJECT_NAME}-chat-server
        chat_message.hpp
        chat_server.cpp
)
target_link_libraries(${PROJECT_NAME}-chat-server PRIVATE asio::asio)
if (WIN32)
    target_compile_definitions(${PROJECT_NAME}-chat-server PRIVATE _WIN32_WINNT=0x0A00)
endif ()


