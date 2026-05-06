#include <asio.hpp>
#include <csignal>
#include <filesystem>
#include <iostream>
#include <string>
#include "FileTransportSession.h"

class FileTransportClient {
public:
    FileTransportClient(asio::io_context& io_context, const std::string& host, const uint16_t port) :
        io_context_(io_context),socket_(io_context), download_dir_(std::filesystem::current_path() / "download") {
        asio::ip::tcp::resolver resolver(io_context);
        endpoints_ = resolver.resolve(host, std::to_string(port));
        std::filesystem::create_directories(download_dir_);
    }

    void do_connect() {
        asio::ip::tcp::socket socket(io_context_);

        asio::async_connect(socket_, endpoints_, [this](const asio::error_code& error, const asio::ip::tcp::endpoint&) {
            if (error) {
                std::cerr << "[Client] Connect error: " << error.message() << std::endl;
                return;
            }

            std::cout << "[Client] Connected to " << socket_.remote_endpoint().address() << ":"
                      << socket_.remote_endpoint().port() << std::endl;
            std::cout << "[Client] Download directory: " << download_dir_.string() << std::endl;
            std::cout << "[Client] Commands: query, upload <filepath>, download <filename>, or any text for echo" << std::endl;
            std::cout << "[Client] Press Ctrl+C to disconnect..." << std::endl;
            std::cout<<"------------------------------------------------------------------"<<std::endl;
            session_ = std::make_shared<FileTransportSession>(std::move(socket_));
            set_session_callback();
            interactive_mode();
        });
    }

private:
    void set_session_callback() {
        if (!session_) {
            std::cerr << "[Client] Session not initialized" << std::endl;
            return;
        }
        session_->set_type_send_complete_callback([this](const bool success) {
            if (!success) {
                std::cerr << "[Client] Failed to send request type" << std::endl;
                return;
            }
            std::cout << "\n[Client] Sent request successfully: " << *session_->get_send_protocol() << std::endl;
            // 发送成功后进入receive状态
            session_->receive_header(download_dir_.string());
        });

        session_->set_type_receive_complete_callback([this](const bool success) {
            if (!success) {
                std::cerr << "[Client] Failed to receive response type" << std::endl;
                return;
            }

            std::cout << "\n[Client] Receive response successfully: " << *session_->get_receive_protocol() << std::endl;
            interactive_mode();
        });

        session_->set_file_receive_complete_callback([this](const bool success) {
            if (success) {
                std::cout << "\n[Client] File downloaded successfully!" << std::flush << std::endl;
            }
            interactive_mode();
        });

        session_->set_file_send_complete_callback([this](const bool success) {
            if (success) {
                std::cout << "\n[Client] File uploaded successfully!" << std::flush << std::endl;
            }
            interactive_mode();
        });

        session_->set_receive_error_callback(
            [](const std::string& error) { std::cerr << "[Client] Receive error: " << error << std::endl; });

        session_->set_send_error_callback(
            [](const std::string& error) { std::cerr << "[Client] Send error: " << error << std::endl; });

        session_->set_send_progress_callback([](const ProgressInfo& info) {
            std::cout << "\r[Client] Upload progress: " << info.toString() << "   " << std::flush;
        });

        session_->set_receive_progress_callback([](const ProgressInfo& info) {
            std::cout << "\r[Client] Download progress: " << info.toString() << "   " << std::flush;
        });
    }

    void interactive_mode() {
        std::string command;
        if (!std::getline(std::cin, command)) {
            session_->close();
            return;
        }
        if (command.empty()) {
            interactive_mode();

        } else if (command == "query") {
            session_->send_header("query", "");

        } else if (command.substr(0, 7) == "upload ") {
            const std::string file_path = command.substr(7);
            upload_file(file_path);

        } else if (command.substr(0, 9) == "download ") {
            const std::string file_name = command.substr(9);
            download_file(file_name);

        } else {
            session_->send_header(command, "");
        }
    }

    void upload_file(const std::string& file_path) const {
        if (file_path.empty()) {
            std::cout << "[Client] Usage: upload <filepath>" << std::endl;
            return;
        }

        if (!std::filesystem::exists(file_path)) {
            std::cout << "[Client] File not found: " << file_path << std::endl;
            return;
        }

        if (!std::filesystem::is_regular_file(file_path)) {
            std::cout << "[Client] Not a regular file: " << file_path << std::endl;
            return;
        }

        std::cout << "[Client] Uploading: " << file_path << std::endl;
        session_->send_header("upload", file_path);
    }

    void download_file(const std::string& file_name) const {
        if (file_name.empty()) {
            std::cout << "[Client] Usage: download <filename>" << std::endl;
            return;
        }

        std::cout << "[Client] Requesting download: " << file_name << std::endl;

        session_->send_header("download", file_name);
    }

    /*void set_on_connected(const std::function<void(bool success)>& cb) { on_connected_cb_ = cb; }
    void set_on_disconnected(const std::function<void(bool success)>& cb) { on_disconnected_cb_ = cb; }

    void report_on_connected_cb(const bool success) const { on_connected_cb_(success); }
    void report_on_disconnected_cb_(const bool success) const { on_disconnected_cb_(success); }
    std::function<void(bool success)> on_connected_cb_;
    std::function<void(bool success)> on_disconnected_cb_;*/

    asio::ip::tcp::socket socket_;
    asio::io_context& io_context_;
    asio::ip::tcp::resolver::results_type endpoints_;
    std::shared_ptr<FileTransportSession> session_;
    std::filesystem::path download_dir_{};
};

int main(const int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        return 1;
    }
    asio::io_context io_context;

    // 设置信号集，捕获 SIGINT (Ctrl+C) 和 SIGTERM
    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    // 异步等待信号
    signals.async_wait([&io_context](const std::error_code& error, const int signal_number) {
        if (!error) {
            std::cout << "\nReceived signal " << signal_number << ". Shutting down..." << std::endl;
            io_context.stop();
        }
    });

    try {
        const std::string host = argv[1];
        const uint16_t port = static_cast<uint16_t>(std::stoi(argv[2]));

        FileTransportClient client(io_context, host, port);
        client.do_connect();

        io_context.run();

    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
