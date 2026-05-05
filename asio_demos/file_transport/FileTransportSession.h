#pragma once
#include <fstream>
#include <functional>
#include <iomanip>
#include <sstream>
#include "FileTransportProtocol.h"
#include "asio.hpp"
#include "chrono"

struct ProgressInfo {
    uint64_t bytes_transferred = 0;
    uint64_t total_bytes = 0;
    double percentage = 0;
    std::chrono::milliseconds elapsed_time{0};
    double speed_kbps = 0; // 原始 KB/s
    double speed_value = 0; // 显示用的数值
    std::string speed_unit = "KB/s";

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "Progress: " << percentage << "% Speed: " << speed_value << " " << speed_unit;
        return ss.str();
    }
};
class FileTransportSession : public std::enable_shared_from_this<FileTransportSession> {
public:
    explicit FileTransportSession(asio::ip::tcp::socket socket);

    void send_header(const std::string& type, const std::string& file_path);
    void receive_header(const std::string& save_dir);

    // set callback
    void set_send_progress_callback(const std::function<void(ProgressInfo&)>& cb) { send_progress_callback_ = cb; }
    void set_send_error_callback(const std::function<void(const std::string&)>& cb) { send_error_callback_ = cb; }
    void set_type_send_complete_callback(const std::function<void(bool success)>& cb) { type_send_complete_callback_ = cb; }
    void set_file_send_complete_callback(const std::function<void(bool success)>& cb) { file_send_complete_callback_ = cb; }

    void set_receive_progress_callback(const std::function<void(ProgressInfo&)>& cb) { receive_progress_callback_ = cb; }
    void set_receive_error_callback(const std::function<void(const std::string&)>& cb) { receive_error_callback_ = cb; }
    void set_type_receive_complete_callback(const std::function<void(bool success)>& cb) { type_receive_complete_callback_ = cb; }
    void set_file_receive_complete_callback(const std::function<void(bool success)>& cb) { file_receive_complete_callback_ = cb; }

    std::shared_ptr<FileTransportProtocol> get_send_protocol() { return send_protocol_; }
    std::shared_ptr<FileTransportProtocol> get_receive_protocol() { return receive_protocol_; }

    void close();
    [[nodiscard]] const asio::ip::tcp::socket& get_socket() const { return socket_; }

private:
    void send_file_truck(uint16_t offset);
    void receive_file_name(const std::string& save_dir, size_t offset);
    void receive_file_truck(size_t offset);

    void report_send_progress(uint64_t transferred, uint64_t total);
    void report_send_error_callback(const std::string& error) const { send_error_callback_(error); }
    void report_type_send_complete_callback(const bool success) const { type_send_complete_callback_(success); }
    void report_file_send_complete_callback(const bool success) const { file_send_complete_callback_(success); }

    void report_receive_progress(uint64_t transferred, uint64_t total);
    void report_receive_error_callback(const std::string& error) const { receive_error_callback_(error); }
    void report_type_receive_complete_callback(const bool success) const { type_receive_complete_callback_(success); }
    void report_file_receive_complete_callback(const bool success) const { file_receive_complete_callback_(success); }

private:
    asio::ip::tcp::socket socket_;
    static constexpr uint16_t BUFFER_SIZE = 32 * 1024; // 32KB 每次传输
    std::ifstream ifs_;
    std::shared_ptr<std::vector<char>> send_buffer_;
    std::shared_ptr<FileTransportProtocol> send_protocol_;
    std::ofstream ofs_;
    std::shared_ptr<std::vector<char>> receive_buffer_;
    std::shared_ptr<FileTransportProtocol> receive_protocol_;

    // 回调函数
    ProgressInfo send_progress_info_;
    std::function<void(ProgressInfo&)> send_progress_callback_;
    std::function<void(const std::string& error)> send_error_callback_;
    std::function<void(bool success)> file_send_complete_callback_;
    std::function<void(bool success)> type_send_complete_callback_;

    ProgressInfo receive_progress_info_;
    std::function<void(ProgressInfo&)> receive_progress_callback_;
    std::function<void(const std::string& error)> receive_error_callback_;
    std::function<void(bool success)> type_receive_complete_callback_;
    std::function<void(bool success)> file_receive_complete_callback_;

    // 文件传输进度信息
    std::chrono::steady_clock::time_point send_start_time_; // 开始时间
    std::chrono::steady_clock::time_point send_last_report_time_; // 上次报告时间
    uint16_t send_last_reported_bytes_ = 0; // 上次报告时的字节数
    uint16_t send_pending_transferred_ = 0; // 待报告的最新字节数（解耦用）
    uint16_t send_bytes_ = 0; // 其他业务变量

    std::chrono::steady_clock::time_point receive_start_time_; // 开始时间
    std::chrono::steady_clock::time_point receive_last_report_time_; // 上次报告时间
    uint16_t receive_last_reported_bytes_ = 0; // 上次报告时的字节数
    uint16_t receive_pending_transferred_ = 0; // 待报告的最新字节数（解耦用）
    uint16_t received_bytes_ = 0; // 其他业务变量
};
