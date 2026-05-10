#include <asio.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <array>

using asio::ip::udp;

std::string make_daytime_string() {
    using namespace std;
    const time_t now = time(0);
    return ctime(&now);
}

int main() {
    try {
        asio::io_context io_context;

        // 创建 UDP socket 并绑定到端口 13
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

        std::cout << "UDP Daytime Server started on port 13" << std::endl;
        std::cout << "Press Ctrl+C to stop." << std::endl;

        std::array<char, 1> recv_buf;
        udp::endpoint remote_endpoint;

        for (;;) {
            try {
                std::cout << "\n[Server] Waiting for incoming request..." << std::endl;
                std::size_t length = socket.receive_from(
                    asio::buffer(recv_buf), remote_endpoint);
                std::cout << "[Server] Received " << length << " byte(s) from client" << std::endl;
                std::cout << "[Server] Client Endpoint: " << remote_endpoint << std::endl;

                std::string daytime = make_daytime_string();
                std::cout << "[Server] Sending response: " << daytime;
                socket.send_to(asio::buffer(daytime), remote_endpoint);

                std::cout << "[Server] Successfully sent daytime to "
                          << remote_endpoint.address().to_string()
                          << ":" << remote_endpoint.port() << std::endl;
            } catch (std::exception& e) {
                std::cerr << "[Server] Error handling client: " << e.what() << std::endl;
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
