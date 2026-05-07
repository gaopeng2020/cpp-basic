project(crypto_demo VERSION 1.0.0 LANGUAGES CXX)

find_package(OpenSSL CONFIG REQUIRED)
# 检查OpenSSL是否找到
if(OPENSSL_FOUND)
    message(STATUS "Found OpenSSL ${OPENSSL_VERSION}")
    message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenSSL not found. Please install libssl-dev")
endif()

add_executable(${PROJECT_NAME}-aes encrypt/aes.cpp)
target_link_libraries(${PROJECT_NAME}-aes  ${OPENSSL_LIBRARIES})
if (WIN32)
    target_link_libraries(${PROJECT_NAME}-aes
            ws2_32      # Windows Socket
            gdi32       # Graphics Device Interface
            crypt32     # Crypto API
    )
endif ()
target_link_libraries(${PROJECT_NAME}-aes
        ${OPENSSL_LIBRARIES}
)

add_executable(${PROJECT_NAME}-gmac mac/gmac.cpp)
target_link_libraries(${PROJECT_NAME}-gmac  ${OPENSSL_LIBRARIES})
if (WIN32)
    target_link_libraries(${PROJECT_NAME}-gmac
            ws2_32      # Windows Socket
            gdi32       # Graphics Device Interface
            crypt32     # Crypto API
    )
endif ()
target_link_libraries(${PROJECT_NAME}-gmac
        ${OPENSSL_LIBRARIES}
)

