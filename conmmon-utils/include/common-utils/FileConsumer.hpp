#pragma once

#include "common-utils/LogConsumer.hpp"

#include <fstream>
namespace common_utils::log {
/**
 * Log consumer that writes the log events to a file.
 *
 * @file FileConsumer.hpp
 */
class FileConsumer : public LogConsumer {
public:
    /** Constructor with parameters.
     * @param filename path of the output file where the log will be wrote.
     * @param log_info Log::LogInfo object with the configuration of the logger.
     * @param append indicates if the consumer must append the content in the filename.
     */
    explicit FileConsumer(const std::string& filename, const Log::LogInfo& log_info, bool append);

    ~FileConsumer() override;
    /**
     * @brief Process and output a log file.
     * @param entry The log entry to consume.
     */
    void output_log(const Log::Entry& entry) override;

protected:
    /** \internal
     * Called by Log consume to get the correct stream
     * @param entry Log::Entry to consume.
     */
    std::ostream& get_stream(const Log::Entry& entry) override;

private:
    FileConsumer() = delete;
    /**
     * @brief Check if file size exceeds limit and rotate if needed.
     */
    void rotate_log_file();

    /**
     * @brief Generate backup filename with timestamp.
     * @return Backup filename with date suffix.
     */
    std::string generate_backup_filename();
    std::string log_file_;
    size_t max_file_size_mb_;

    std::ofstream ofs_;
    bool append_;

    int flush_interval_sec_ = 0;
    std::chrono::system_clock::time_point last_flush_time_;
    std::ostringstream buffer_;
    std::mutex buffer_mutex_;
};
} // namespace common_utils::log
