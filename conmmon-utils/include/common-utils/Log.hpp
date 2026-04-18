#pragma once

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <regex>
#include <sstream>
#include <thread>
#include <vector>
#include "common-utils/common-utilsExport.h"
namespace common_utils::log {
/**
 * @file Log.hpp
 * @brief Asynchronous logging system with thread-safe queue and configurable consumers.
 */

/**
 * Logging verbosity can be statically disabled by defining:
 * - LOG_NO_ERROR
 * - LOG_NO_WARNING
 * - LOG_NO_INFO
 */

#define log_debug(cat, msg) LOG_DEBUG_IMPL_(cat, msg)
#define log_info(cat, msg) LOG_INFO_IMPL_(cat, msg)
#define log_warning(cat, msg) LOG_WARNING_IMPL_(cat, msg)
#define log_error(cat, msg) LOG_ERROR_IMPL_(cat, msg)

// 不能直接include LogConsumer.hpp会造成循环依赖，告诉编译器："有这个类，定义在后面"
class LogConsumer;

/**
 * @class Log
 * @brief Thread-safe asynchronous logging system with configurable output consumers.
 *
 * Provides three log levels (Error, Warning, Info) with support for multiple output destinations,
 * category/filename filtering, and automatic thread lifecycle management.
 */
class COMMON_UTILS_EXPORT Log {
public:
    /**
     * @enum Kind
     * @brief Log message severity levels.
     */
    enum Kind : uint8_t {
        Error, ///< Highest priority, cannot be disabled at runtime
        Warning, ///< Medium priority
        Info, ///< Lowest priority, useful for debugging
        Debug ///< Debugging messages
    };

    /**
     * @struct Context
     * @brief Source code location information for log entries.
     */
    struct Context {
        const char* filename; ///< Source file name
        int line; ///< Line number in source file
        const char* function; ///< Function name
        const char* category; ///< User-defined log category
    };

    /**
     * @struct Entry
     * @brief Complete log message with metadata.
     */
    struct Entry {
        std::string message; ///< Log message content
        Context context; ///< Source code context
        Kind kind; ///< Message severity
        std::string timestamp; ///< ISO-formatted timestamp
    };

    struct LogInfo {
        std::string app_name;
        std::string verbosity;
        bool use_log_file;
        int flush_interval_sec;
        int max_file_size_mb;
    };

    /**
     * @brief Initialize the logging system.
     * @param app_name Application name used in log banner.
     * @param verbosity Minimum log level (default: Info).
     * @param use_console Enable console output (default: true).
     * @param flush_interval_sec Auto-flush interval in seconds (default: 5).
     * @param max_file_size_mb Maximum log file size in megabytes (default: 100MB).
     * @param use_log_file Enable log file output (default: false).
     */
    static void init(const std::string& app_name, Kind verbosity = Info, bool use_console = true, bool use_log_file = false,
                     int flush_interval_sec = 5, int max_file_size_mb = 100);
    /**
     * @brief Check if logging system has been initialized.
     * @return true if initialized, false otherwise.
     */
    static bool is_initialized();

    /**
     * @brief Shutdown the logging system.
     */
    static void shutdown();
    /**
     * @brief Register a custom output consumer.
     * @param consumer Unique pointer to LogConsumer instance (ownership transferred).
     */
    static void register_consumer(std::unique_ptr<LogConsumer>&& consumer);

    /**
     * @brief Remove all registered consumers including default stdout consumer.
     */
    static void clear_consumers();

    /**
     * @brief Block until all queued log messages are processed.
     */
    static void flush();

    /**
     * @brief Internal method to enqueue log messages. Use LOG_INFO/WARNING/ERROR macros instead.
     * @param message Log message text.
     * @param context Source code location information.
     * @param kind Message severity level.
     */
    static void queue_Log(const std::string& message, const Context& context, Kind kind);
    /**
     * @brief Enable or disable filename reporting in log entries.
     * @param report true to show filenames, false to hide (default: false).
     */
    static void report_filenames(bool report);

    /**
     * @brief Enable or disable function name reporting in log entries.
     * @param report true to show function names, false to hide (default: true).
     */
    static void report_functions(bool report);

    /**
     * @brief Set minimum verbosity level for log messages.
     * @param kind Minimum severity level to log (default: Error).
     */
    static void set_verbosity(Kind kind);

    /**
     * @brief Get current verbosity level.
     * @return Current minimum severity level.
     */
    static Kind get_verbosity();

    /**
     * @brief Set regex filter for log categories.
     * @param filter Regular expression to match against category names.
     */
    static void set_category_filter(const std::regex& filter);

    /**
     * @brief Remove category filter.
     */
    static void unset_category_filter();

    /**
     * @brief Check if category filter is active.
     * @return true if filter is set, false otherwise.
     */
    static bool has_category_filter();

    /**
     * @brief Get current category filter.
     * @return Copy of current regex filter or empty regex if not set.
     */
    static std::regex get_category_filter();

private:
    Log();
    ~Log();

    static Log& get_instance();

    static std::string get_timestamp();
    void start_thread();
    void run();
    bool preprocess(Entry& entry) const;

    std::vector<std::unique_ptr<LogConsumer>> consumers_;

    std::thread logging_thread_;
    std::atomic<bool> logging_;

    std::queue<Entry> log_queue_;
    std::mutex queue_mutex_;

    std::condition_variable cv_;
    std::mutex cv_mutex_;
    bool flush_requested_ = false;

    std::mutex config_mutex_;
    bool filenames_;
    bool functions_;
    std::unique_ptr<std::regex> category_filter_;

    Kind verbosity_;
    std::string app_name_;
    bool initialized_ = false;
};

/**
 * @brief Stream operator for Log::Kind enumeration.
 * @param output Output stream.
 * @param kind Log severity level.
 * @return Reference to output stream.
 */
inline std::ostream& operator<<(std::ostream& output, const Log::Kind& kind) {
    switch (kind) {
        case Log::Kind::Info:
            output << "INFO";
            break;
        case Log::Kind::Warning:
            output << "WARNING";
            break;
        case Log::Kind::Error:
            output << "ERROR";
            break;
        case Log::Kind::Debug:
            output << "DEBUG";
            break;
        default:
            output << "Invalid Verbosity Kind.";
            break;
    }
    return output;
}

#if defined(WIN32)
#    define _func_ __FUNCTION__
#endif

#define HAVE_LOG_NO_ERROR 0
#define HAVE_LOG_NO_WARNING 0
#define HAVE_LOG_NO_INFO 0
#define HAVE_LOG_NO_DEBUG 0

#if !HAVE_LOG_NO_ERROR
#    define LOG_ERROR_IMPL_(cat, msg)                                                                                            \
        do {                                                                                                                     \
            std::stringstream log_ss_tmp__;                                                                                      \
            log_ss_tmp__ << msg;                                                                                                 \
            Log::queue_Log(log_ss_tmp__.str(), Log::Context{__FILE__, __LINE__, __func__, #cat}, Log::Kind::Error);              \
        } while (0)
#else
#    define LOG_ERROR_IMPL_(cat, msg)
#endif

#if !HAVE_LOG_NO_WARNING
#    define LOG_WARNING_IMPL_(cat, msg)                                                                                          \
        do {                                                                                                                     \
            if (Log::get_verbosity() >= Log::Kind::Warning) {                                                                    \
                std::stringstream log_ss_tmp__;                                                                                  \
                log_ss_tmp__ << msg;                                                                                             \
                Log::queue_Log(log_ss_tmp__.str(), Log::Context{__FILE__, __LINE__, __func__, #cat}, Log::Kind::Warning);        \
            }                                                                                                                    \
        } while (0)
#else
#    define LOG_WARNING_IMPL_(cat, msg)
#endif

#if !HAVE_LOG_NO_INFO
#    define LOG_INFO_IMPL_(cat, msg)                                                                                             \
        do {                                                                                                                     \
            if (Log::get_verbosity() >= Log::Kind::Info) {                                                                       \
                std::stringstream log_ss_tmp__;                                                                                  \
                log_ss_tmp__ << msg;                                                                                             \
                Log::queue_Log(log_ss_tmp__.str(), Log::Context{__FILE__, __LINE__, __func__, #cat}, Log::Kind::Info);           \
            }                                                                                                                    \
        } while (0)
#else
#    define LOG_INFO_IMPL_(cat, msg)
#endif

#if !HAVE_LOG_NO_DEBUG
#    define LOG_DEBUG_IMPL_(cat, msg)                                                                                            \
        do {                                                                                                                     \
            if (Log::get_verbosity() >= Log::Kind::Debug) {                                                                      \
                std::stringstream log_ss_tmp__;                                                                                  \
                log_ss_tmp__ << msg;                                                                                             \
                Log::queue_Log(log_ss_tmp__.str(), Log::Context{__FILE__, __LINE__, __func__, #cat}, Log::Kind::Debug);          \
            }                                                                                                                    \
        } while (0)
#else
#    define LOG_DEBUG_IMPL_(cat, msg)
#endif
} // namespace common_utils::log
