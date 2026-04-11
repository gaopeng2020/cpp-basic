#include "common-utils/FileConsumer.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
#    include <windows.h>
#else
#    include <unistd.h>
#endif
namespace common_utils::log {
FileConsumer::FileConsumer(const std::string& filename, const Log::LogInfo& log_info, const bool append) :
    log_file_(filename),
    max_file_size_mb_(log_info.max_file_size_mb),
    append_(append),
    flush_interval_sec_(log_info.flush_interval_sec) {
    ofs_.open(log_file_, append_ ? std::ios::app : std::ios::trunc);
    if (!ofs_.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
    last_flush_time_ = std::chrono::system_clock::now();

    ofs_ << "===================================================================================\n";
    ofs_ << "  Logging System Initialized\n";
    ofs_ << "===================================================================================\n";
    ofs_ << "  Application : " << log_info.app_name << "\n";
    ofs_ << "  Verbosity   : " << log_info.verbosity << "\n";
    ofs_ << "  File Logger : " << (log_info.use_log_file ? ": yes" : ": no") << "\n";
    ofs_ << "  Flush Interval(s)  : " << (log_info.flush_interval_sec > 0 ? std::to_string(log_info.flush_interval_sec) : "0")
         << "s" << "\n";
    ofs_ << "  Max Log file size : " << (log_info.max_file_size_mb > 0 ? std::to_string(log_info.max_file_size_mb) : "100")
         << "MB" << "\n";
    ofs_ << "====================================================================================\n";
    ofs_.flush();
}

FileConsumer::~FileConsumer() {
    if (ofs_.is_open()) {
        // Flush remaining buffered logs
        std::lock_guard<std::mutex> lock(buffer_mutex_);
        if (buffer_.tellp() > 0) {
            ofs_ << buffer_.str();
            ofs_.flush();
        }
        ofs_ << "=================================================================================\n";
        ofs_ << "  Logging System Shutdown\n";
        ofs_ << "=================================================================================\n\n";
        ofs_.flush();
        ofs_.close();
    }
}
std::string FileConsumer::generate_backup_filename() {
    const auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_buf);
#endif

    std::ostringstream oss;
    oss << log_file_ << "." << (tm_buf.tm_year + 1900) << std::setfill('0') << std::setw(2) << (tm_buf.tm_mon + 1)
        << std::setfill('0') << std::setw(2) << tm_buf.tm_mday << "_" << std::setfill('0') << std::setw(2) << tm_buf.tm_hour
        << std::setfill('0') << std::setw(2) << tm_buf.tm_min << std::setfill('0') << std::setw(2) << tm_buf.tm_sec << ".bak";

    return oss.str();
}
void FileConsumer::rotate_log_file() {
    if (max_file_size_mb_ == 0) {
        return;
    }

    ofs_.flush();

    struct stat file_stat;
    if (stat(log_file_.c_str(), &file_stat) != 0) {
        return;
    }

    const size_t file_size_bytes = static_cast<size_t>(file_stat.st_size);
    if (const size_t max_size_bytes = max_file_size_mb_ * 1024 * 1024; file_size_bytes >= max_size_bytes) {
        ofs_.close();

        const std::string backup_name = generate_backup_filename();

#ifdef _WIN32
        MoveFileA(log_file_.c_str(), backup_name.c_str());
#else
        rename(log_file_.c_str(), backup_name.c_str());
#endif

        std::cout << "Log file rotated: " << backup_name << std::endl;

        ofs_.open(log_file_, std::ios::trunc);
        if (!ofs_.is_open()) {
            std::cerr << "Failed to create new log file: " << log_file_ << std::endl;
        }

        ofs_ << "===================================================================================\n";
        ofs_ << "  Log file rotated. Previous backup: " << backup_name << "\n";
        ofs_ << "===================================================================================\n\n";
        ofs_.flush();
    }
}
void FileConsumer::output_log(const Log::Entry& entry) {
    std::lock_guard lock(buffer_mutex_);
    // Format log entry
    print_timestamp(buffer_, entry, false);
    print_header(buffer_, entry, false);
    print_message(buffer_, entry, false);
    print_context(buffer_, entry, false);
    print_new_line(buffer_, false);

    if (flush_interval_sec_ > 0) {
        const auto now = std::chrono::system_clock::now();
        if (const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_flush_time_).count();
            elapsed >= flush_interval_sec_) {
            ofs_ << buffer_.str();
            ofs_.flush();
            buffer_.str("");
            buffer_.clear();
            last_flush_time_ = now;
        }
    } else {
        ofs_ << buffer_.str();
        ofs_.flush();
        buffer_.str("");
        buffer_.clear();
    }
    // Rotate log file if needed
    rotate_log_file();
}

std::ostream& FileConsumer::get_stream(const Log::Entry& entry) {
    (void)entry;
    return ofs_;
}
} // namespace common_utils::log