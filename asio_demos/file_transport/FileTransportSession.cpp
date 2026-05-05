#include "FileTransportSession.h"
#include <filesystem>
#include <iostream>

FileTransportSession::FileTransportSession(asio::ip::tcp::socket socket) :
    socket_(std::move(socket)),
    send_buffer_(std::make_shared<std::vector<char>>(BUFFER_SIZE)),
    send_protocol_(std::make_shared<FileTransportProtocol>()),
    receive_buffer_(std::make_shared<std::vector<char>>(BUFFER_SIZE)),
    receive_protocol_(std::make_shared<FileTransportProtocol>()) {}

void FileTransportSession::send_header(const std::string& type, const std::string& file_path) {
    // 重置协议对象和缓冲区，避免残留数据
    send_protocol_->reset();

    if (type.empty()) return;
    send_protocol_->type = type;
    send_protocol_->type_len = type.size();
    send_protocol_->file_name_len = 0;
    if (!file_path.empty() && std::filesystem::is_regular_file(file_path)) {
        const std::filesystem::path path = file_path;
        send_protocol_->file_name = path.filename().string();
        send_protocol_->file_name_len = send_protocol_->file_name.size();
        send_protocol_->file_size = std::filesystem::file_size(path);
    }
    if (!send_protocol_->encode_header(send_buffer_->data())) {
        report_send_error_callback("type or file name was too long ,shall be less than 65535 bytes");
        return;
    }
    auto self(shared_from_this());
    const uint16_t offset = send_protocol_->get_header_len();
    asio::async_write(socket_, asio::buffer(send_buffer_->data(), offset),
                      [this, file_path, offset, self](const asio::error_code& error, size_t) {
                          if (error) {
                              std::cerr << "[Session] send header error" << std::endl;
                              report_send_error_callback(error.message());
                              if (ifs_.is_open()) ifs_.close();
                              return;
                          }
                          if (send_protocol_->file_name_len == 0 || send_protocol_->file_size == 0) {
                              report_type_send_complete_callback(true);
                              return;
                          }

                          // 打开文件用于读取
                          ifs_.open(file_path, std::ios::binary);
                          if (!ifs_.is_open()) {
                              report_send_error_callback("Failed to open file: " + file_path);
                              return;
                          }
                          send_bytes_ = 0;
                          send_start_time_ = std::chrono::steady_clock::now();
                          send_last_report_time_ = send_start_time_;
                          send_last_reported_bytes_ = 0;

                          // 开始发送文件数据块
                          send_file_truck(offset);
                      });
}

void FileTransportSession::send_file_truck(const uint16_t offset) {
    ifs_.read(send_buffer_->data() + offset, BUFFER_SIZE - offset);
    const size_t bytes_read = ifs_.gcount();

    if (bytes_read == 0) {
        // 文件读取完成
        ifs_.close();
        report_file_send_complete_callback(true);
        return;
    }

    send_bytes_ += bytes_read;

    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(send_buffer_->data() + offset, bytes_read),
                      [this, self](const asio::error_code& error, size_t) {
                          if (error) {
                              std::cerr << "[Session] send file truck error" << std::endl;
                              report_send_error_callback(error.message());
                              // if (ifs_.is_open()) ifs_.close();
                              return;
                          }

                          // 报告进度
                          report_send_progress(send_bytes_, send_protocol_->file_size);

                          // 继续发送下一块
                          // send_buffer_->clear();
                          send_file_truck(0);
                      });
}

void FileTransportSession::receive_header(const std::string& save_dir) {
    // 重置协议对象和缓冲区，避免残留数据
    receive_protocol_->reset();

    auto self(shared_from_this());
    // 首先读取 type_len
    asio::async_read(socket_, asio::buffer(receive_buffer_->data(), sizeof(uint16_t)),
                     [this, self, save_dir](const asio::error_code& error, size_t) {
                         if (error) {
                             std::cerr << "[Session] receive type length error" << std::endl;
                             report_receive_error_callback(error.message());
                             return;
                         }

                         receive_protocol_->decode_header_len(receive_buffer_->data(), true);

                         // 读取 type 内容 + file_name_len
                         size_t start_pos = sizeof(receive_protocol_->type_len);
                         const size_t read_bytes = receive_protocol_->type_len + sizeof(receive_protocol_->file_name_len);
                         if (read_bytes == 0) { // 空消息，直接报告完成
                             report_type_receive_complete_callback(true);
                             return;
                         }

                         asio::async_read(socket_, asio::buffer(receive_buffer_->data() + start_pos, read_bytes),
                                          [this, self, save_dir, start_pos](const asio::error_code& ec, size_t) {
                                              if (ec) {
                                                  std::cerr << "[Session] receive type and file name length error" << std::endl;
                                                  report_receive_error_callback(ec.message());
                                                  return;
                                              }

                                              // 解析 type
                                              receive_protocol_->decode_header(receive_buffer_->data() + start_pos);
                                              size_t offset = start_pos + receive_protocol_->type_len;

                                              // 解析 file_name_len
                                              receive_protocol_->decode_header_len(receive_buffer_->data() + offset, false);

                                              // 如果有文件名，继续接收文件
                                              if (receive_protocol_->file_name_len == 0) {
                                                  report_type_receive_complete_callback(true);
                                                  return;
                                              }
                                              offset += receive_protocol_->file_name_len;
                                              receive_file_name(save_dir, offset);
                                          });
                     });
}

void FileTransportSession::receive_file_name(const std::string& save_dir, const size_t offset) {
    std::cout << "[Debug] receive_file_name called, offset=" << offset << ", file_name_len=" << receive_protocol_->file_name_len
              << ", file_name=" << receive_protocol_->file_name << std::endl;

    // 读取 file_name + file_size
    const size_t read_bytes = receive_protocol_->file_name_len + sizeof(receive_protocol_->file_size);

    auto self(shared_from_this());
    asio::async_read(socket_, asio::buffer(receive_buffer_->data() + offset, read_bytes),
                     [this, save_dir, offset, read_bytes, self](const asio::error_code& error, size_t) {
                         if (error) {
                             std::cerr << "[Session] receive file name and file size error" << std::endl;
                             report_receive_error_callback(error.message());
                             return;
                         }

                         // 解析 file_name 与file_size
                         receive_protocol_->decode_header(receive_buffer_->data() + offset);

                         // 判断file size
                         if (receive_protocol_->file_size == 0) {
                             report_type_receive_complete_callback(true);
                             return;
                         }

                         // 创建保存目录和文件
                         const std::filesystem::path path = std::filesystem::path(save_dir) / receive_protocol_->file_name;
                         std::filesystem::create_directories(path.parent_path());

                         // 打开输出文件流
                         ofs_.open(path.string(), std::ios::binary);
                         if (!ofs_.is_open()) {
                             report_receive_error_callback("Failed to create file: " + path.string());
                             return;
                         }

                         received_bytes_ = 0;
                         receive_start_time_ = std::chrono::steady_clock::now();
                         receive_last_report_time_ = receive_start_time_;
                         receive_last_reported_bytes_ = 0;

                         // 开始接收文件数据
                         receive_file_truck(offset + read_bytes);
                     });
}

void FileTransportSession::receive_file_truck(size_t offset) {
    const size_t remaining = receive_protocol_->file_size - received_bytes_;
    const size_t read_bytes = remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE;

    if (read_bytes == 0) {
        // 接收完成
        report_file_receive_complete_callback(true);
        ofs_.close();
        return;
    }

    auto self(shared_from_this());
    asio::async_read(socket_, asio::buffer(receive_buffer_->data() + offset, read_bytes),
                     [this, offset, self](const asio::error_code& error, const size_t bytes_transferred) {
                         if (error) {
                             std::cerr << "[Session] receive file truck error" << std::endl;
                             report_receive_error_callback(error.message());
                             // if (ofs_.is_open()) ofs_.close();
                             return;
                         }

                         // 写入文件
                         ofs_.write(receive_buffer_->data() + offset, static_cast<std::streamsize>(bytes_transferred));
                         received_bytes_ += bytes_transferred;

                         // 报告进度
                         report_receive_progress(received_bytes_, receive_protocol_->file_size);

                         // 继续接收下一块
                         // receive_buffer_->clear();
                         receive_file_truck(0);
                     });
}

void FileTransportSession::close() {
    socket_.close();
    if (ifs_.is_open()) ifs_.close();
    if (ofs_.is_open()) ofs_.close();
}

void FileTransportSession::report_send_progress(const uint64_t transferred, const uint64_t total) {
    if (!send_progress_callback_) return;

    // 记录最新值，与调用频率解耦
    send_pending_transferred_ = transferred;

    const auto now = std::chrono::steady_clock::now();

    // 频率控制：首次或达到间隔或传输完成才报告
    if (const bool is_first_report = send_last_reported_bytes_ == 0; !is_first_report && send_pending_transferred_ < total) {
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - send_last_report_time_).count();
        constexpr uint64_t GB = 1024 * 1024 * 1024;
        if (const int64_t interval_ms = total < GB ? 200 : 1000; elapsed_ms < interval_ms) return;
    }

    // 计算速度（使用平均速度）
    const auto total_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - send_start_time_).count();
    const double total_elapsed_sec = static_cast<double>(total_elapsed) / 1000.0;
    const double speed_kbps =
        total_elapsed_sec > 0 ? static_cast<double>(send_pending_transferred_) / 1024.0 / total_elapsed_sec : 0.0;

    // 单位选择：≥100MB 或 ≥1MB/s 时显示 MB/s
    constexpr uint64_t HUNDRED_MB = 100 * 1024 * 1024;
    const bool use_mb = total >= HUNDRED_MB || speed_kbps >= 1024.0;

    // 复用 ProgressInfo 对象
    send_progress_info_.bytes_transferred = send_pending_transferred_;
    send_progress_info_.total_bytes = total;
    send_progress_info_.percentage =
        total == 0 ? 0.0 : 100.0 * static_cast<double>(send_pending_transferred_) / static_cast<double>(total);
    send_progress_info_.elapsed_time = std::chrono::milliseconds(total_elapsed);
    send_progress_info_.speed_kbps = speed_kbps;
    send_progress_info_.speed_value = use_mb ? speed_kbps / 1024.0 : speed_kbps;
    send_progress_info_.speed_unit = use_mb ? "MB/s" : "KB/s";

    send_progress_callback_(send_progress_info_);

    // 更新报告状态
    send_last_reported_bytes_ = send_pending_transferred_;
    send_last_report_time_ = now;
}
void FileTransportSession::report_receive_progress(const uint64_t transferred, const uint64_t total) {
    if (!receive_progress_callback_) return;

    // 记录最新值，与调用频率解耦
    receive_pending_transferred_ = transferred;

    const auto now = std::chrono::steady_clock::now();

    // 频率控制：首次或达到间隔或传输完成才报告
    if (const bool is_first_report = receive_last_reported_bytes_ == 0;
        !is_first_report && receive_pending_transferred_ < total) {
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - receive_last_report_time_).count();
        constexpr uint64_t GB = 1024 * 1024 * 1024;
        if (const int64_t interval_ms = total < GB ? 200 : 1000; elapsed_ms < interval_ms) return;
    }

    // 计算速度（使用平均速度）
    const auto total_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - receive_start_time_).count();
    const double total_elapsed_sec = static_cast<double>(total_elapsed) / 1000.0;
    const double speed_kbps =
        total_elapsed_sec > 0 ? static_cast<double>(receive_pending_transferred_) / 1024.0 / total_elapsed_sec : 0.0;

    // 单位选择：≥100MB 或 ≥1MB/s 时显示 MB/s
    constexpr uint64_t HUNDRED_MB = 100 * 1024 * 1024;
    const bool use_mb = total >= HUNDRED_MB || speed_kbps >= 1024.0;

    // 复用 ProgressInfo 对象
    receive_progress_info_.bytes_transferred = receive_pending_transferred_;
    receive_progress_info_.total_bytes = total;
    receive_progress_info_.percentage =
        total == 0 ? 0.0 : 100.0 * static_cast<double>(receive_pending_transferred_) / static_cast<double>(total);
    receive_progress_info_.elapsed_time = std::chrono::milliseconds(total_elapsed);
    receive_progress_info_.speed_kbps = speed_kbps;
    receive_progress_info_.speed_value = use_mb ? speed_kbps / 1024.0 : speed_kbps;
    receive_progress_info_.speed_unit = use_mb ? "MB/s" : "KB/s";

    receive_progress_callback_(receive_progress_info_);

    // 更新报告状态
    receive_last_reported_bytes_ = receive_pending_transferred_;
    receive_last_report_time_ = now;
}
