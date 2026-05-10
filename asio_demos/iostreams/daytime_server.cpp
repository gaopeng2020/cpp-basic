#include <asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

using asio::ip::tcp;

std::string make_daytime_string() {
    using namespace std; // For time_t, time and ctime;
    const time_t now = time(0);
    return ctime(&now);
}

int main() {
    try {
        asio::io_context io_context;

        tcp::endpoint endpoint(tcp::v4(), 13);
        tcp::acceptor acceptor(io_context, endpoint);

        for (;;) {
            tcp::iostream stream;
            std::error_code ec;
            acceptor.accept(stream.socket(), ec);
            if (!ec) {
                stream << make_daytime_string();
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
