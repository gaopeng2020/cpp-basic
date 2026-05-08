project(crypto_demo VERSION 1.0.0 LANGUAGES CXX)
find_package(OpenSSL CONFIG REQUIRED)

add_executable(${PROJECT_NAME}-aes encrypt/aes.cpp)
target_link_libraries(${PROJECT_NAME}-aes OpenSSL::Crypto)

add_executable(${PROJECT_NAME}-gmac mac/gmac.cpp)
target_link_libraries(${PROJECT_NAME}-gmac OpenSSL::Crypto)

