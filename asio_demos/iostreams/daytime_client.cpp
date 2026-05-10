#include <asio.hpp>
#include <iostream>
#include <string>

using asio::ip::tcp;

int main(const int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: daytime_client <host>" << std::endl;
            return 1;
        }

        tcp::iostream s(argv[1], "daytime");
        if (!s) {
            std::cout << "Unable to connect: " << s.error().message() << std::endl;
            return 1;
        }

        std::string line;
        std::getline(s, line);
        std::cout << line << std::endl;
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
