#include <asio.hpp>
#include <csignal>
#include <filesystem>
#include <iostream>
#include <map>
#include <set>
#include "FileTransportSession.h"

namespace fs = std::filesystem;

class FileTransportServer {
public:
    explicit FileTransportServer(asio::io_context& io_context, const uint16_t port) :
        acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), receive_dir_(fs::current_path() / "receive") {
        fs::create_directories(receive_dir_);
        std::cout << "[Server] Listening on port " << acceptor_.local_endpoint().port() << std::endl;
        std::cout << "[Server] Receive directory: " << receive_dir_.string() << std::endl;
        std::cout << "[Server] Press Ctrl+C to stop..." << std::endl;
        start_accept();
    }

    void start_accept() {
        acceptor_.async_accept([this](const asio::error_code& error, asio::ip::tcp::socket socket) {
            if (error) std::cerr << "[Server] Accept error: " << error.message() << std::endl;

            std::cout << "[Server] Client connected successfully: " << socket.remote_endpoint().address() << ":"
                      << socket.remote_endpoint().port() << std::endl;
            std::cout << "------------------------------------------------------------------" << std::endl;
            const auto session = std::make_shared<FileTransportSession>(std::move(socket));
            set_session_callback(session);
            // interactive_mode(session);
            // 开始接收头部
            asio::post(acceptor_.get_executor(), [session, this]() { session->receive_header(receive_dir_.string()); });

            start_accept();
        });
    }

private:
    void set_session_callback(const std::shared_ptr<FileTransportSession>& session) const {
        session->set_type_send_complete_callback([this, session](const bool success) {
            if (!success) {
                std::cerr << "[Server] Failed to send request type" << std::endl;
                return;
            }
            std::cout << "\n[Server] Sent request successfully: " << *session->get_send_protocol() << std::endl;
            // 发送完成后，继续监听下一个请求
            session->receive_header(receive_dir_.string());
        });

        session->set_type_receive_complete_callback([this, session](const bool success) {
            if (!success) {
                std::cerr << "[Server] Failed to receive type" << std::endl;
                return;
            }

            const std::string address = session->get_socket().remote_endpoint().address().to_string() + ":" +
                std::to_string(session->get_socket().remote_endpoint().port());
            std::cout << "[Server] Received from " << address << ", and header is :" << *session->get_receive_protocol()
                      << std::endl;

            if (const std::string& type = session->get_receive_protocol()->type; type == "query") {
                handle_query(session);

            } else if (type == "upload") {
                // handle_upload(session);

            } else if (type == "download") {
                handle_download_request(session);

            } else {
                const std::string response = "Echo: " + type;
                std::cout << "[Server] Echoing: " << type << std::endl;
                session->send_header(response, "");
            }
        });

        session->set_file_receive_complete_callback([this, session](const bool success) {
            if (!success) {
                std::cerr << "[Server] Failed to receive type" << std::endl;
                return;
            }
            // 文件接收完成后，继续监听
            session->receive_header(receive_dir_.string());
        });

        session->set_file_send_complete_callback([this, session](const bool success) {
            if (success) {
                std::cout << "\n[Server] File uploaded successfully!" << std::flush << std::endl;
            }
            // 文件发送完成后，继续监听下一个请求
            session->receive_header(receive_dir_.string());
        });

        session->set_receive_error_callback(
            [session](const std::string& error) { std::cerr << "[Server] Receive error: " << error << std::endl; });

        session->set_send_error_callback(
            [session](const std::string& error) { std::cerr << "[Server] Send error: " << error << std::endl; });

        session->set_send_progress_callback([session](const ProgressInfo& info) {
            std::cout << "\r[Server] Upload progress: " << info.toString() << "   " << std::flush;
        });

        session->set_receive_progress_callback([session](const ProgressInfo& info) {
            std::cout << "\r[Server] Download progress: " << info.toString() << "   " << std::flush;
        });
    }

    void interactive_mode(const std::shared_ptr<FileTransportSession>& session) const {
        // 设置回调
        session->set_type_receive_complete_callback([this, session](const bool success) {
            if (!success) {
                std::cerr << "[Server] Failed to receive type" << std::endl;
                return;
            }
            const std::string address = session->get_socket().remote_endpoint().address().to_string() + ":" +
                std::to_string(session->get_socket().remote_endpoint().port());
            std::cout << "[Server] Received from " << address << ", and header is :" << *session->get_receive_protocol()
                      << std::endl;

            if (const std::string& type = session->get_receive_protocol()->type; type == "query") {
                handle_query(session);

            } else if (type == "upload") {

            } else if (type == "download") {
                handle_download_request(session);

            } else {
                const std::string response = "Echo: " + type;
                std::cout << "[Server] Echoing: " << type << std::endl;
                session->send_header(response, "");
            }
        });
        // 开始接收头部
        session->receive_header(receive_dir_.string());
    }

    void handle_query(const std::shared_ptr<FileTransportSession>& session) const {
        std::string file_list;
        if (fs::exists(receive_dir_)) {
            for (const auto& entry : fs::directory_iterator(receive_dir_)) {
                if (entry.is_regular_file()) {
                    std::string fileName = entry.path().filename().string();
                    file_list += fileName + "\n";
                }
            }
        }

        if (file_list.empty()) {
            file_list = "(no files available)\n";
        }
        session->send_header(file_list, "");
    }

    void handle_download_request(const std::shared_ptr<FileTransportSession>& session) const {
        const std::string& file_name = session->get_receive_protocol()->file_name;
        std::cout << "[Server] Client requested download: " << file_name << std::endl;

        const fs::path file_path = receive_dir_ / file_name;

        if (!fs::exists(file_path) || !fs::is_regular_file(file_path)) {
            std::string error_msg = std::format("请求的文件:{} 不存在", file_name);
            std::cout << "[Server] " << error_msg << std::endl;
            session->send_header("error", "");
            return;
        }
        std::cout << "[Server] Sending file: " << file_name << std::endl;
        session->send_header("download_response", file_path.string());
    }

    asio::ip::tcp::acceptor acceptor_;
    fs::path receive_dir_;
    // asio::io_context io_context_;
    // std::map<std::string, std::shared_ptr<FileTransportSession>> sessions_;
};

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    asio::io_context io_context;
    // 设置信号集，捕获 SIGINT (Ctrl+C) 和 SIGTERM
    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&io_context](const std::error_code& error, const int signal_number) {
        if (!error) {
            std::cout << "\nReceived signal " << signal_number << ". Shutting down..." << std::endl;
            io_context.stop();
        }
    });

    try {
        const uint16_t port = static_cast<uint16_t>(std::stoi(argv[1]));
        FileTransportServer server(io_context, port);

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
