#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

#include "common-utils/FileConsumer.hpp"
#include "common-utils/Log.hpp"
#include "common-utils/LogConsumer.hpp"
#include "common-utils/StdoutConsumer.hpp"

#ifdef _WIN32
#    include <windows.h>
#endif
namespace common_utils::log {
// 在 Windows 上启用 ANSI 颜色
#ifdef _WIN32
static void enable_ansi_colors() {
    if (const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
}
#endif

Log::Log() : logging_(false), filenames_(false), functions_(true), verbosity_(Error) {
#ifdef _WIN32
    enable_ansi_colors();
#endif
}

Log::~Log() {
    if (logging_) {
        flush();
        { // ← 这个大括号创建了一个作用域块
            std::unique_lock guard(cv_mutex_);
            logging_ = false;
        } // ← guard 在这里析构，释放锁
        cv_.notify_all();

        if (logging_thread_.joinable()) {
            logging_thread_.join();
        }
    }
}

Log& Log::get_instance() {
    static Log instance;
    return instance;
}

void Log::init(const std::string& app_name, const Kind verbosity, const bool use_console, const bool use_log_file,
               const int flush_interval_sec, const int max_file_size_mb) {
    Log& instance = get_instance();

    std::lock_guard guard(instance.config_mutex_);
    if (instance.initialized_) {
        std::cerr << "Warning: Log::init() called multiple times!" << std::endl;
        return;
    }

    instance.app_name_ = app_name;
    instance.verbosity_ = verbosity;
    instance.initialized_ = true;

    auto verbosity_str = "";
    switch (verbosity) {
        case Error:
            verbosity_str = "ERROR";
            break;
        case Warning:
            verbosity_str = "WARNING";
            break;
        case Info:
            verbosity_str = "INFO";
            break;
        case Debug:
            verbosity_str = "DEBUG";
            break;
    }
    const LogInfo log_info = {app_name, verbosity_str, use_log_file, flush_interval_sec, max_file_size_mb};

    // create console log consumer
    if (use_console) instance.consumers_.emplace_back(new StdoutConsumer(log_info));

    // create file log consumer
    if (use_log_file) instance.consumers_.emplace_back(new FileConsumer("log.log", log_info, true));
}

bool Log::is_initialized() { return get_instance().initialized_; }

void Log::shutdown() {
    Log& instance = get_instance();

    if (!instance.initialized_) {
        return;
    }
    // print shutdown banner
    std::cout << "================================================================\n";
    std::cout << "  Logging System Shutdown\n";
    std::cout << "================================================================\n\n";

    flush();
    {
        std::unique_lock guard(instance.cv_mutex_);
        instance.logging_ = false;
    }
    instance.cv_.notify_all();

    if (instance.logging_thread_.joinable()) {
        instance.logging_thread_.join();
    }

    instance.initialized_ = false;
}

void Log::queue_Log(const std::string& message, const Context& context, const Kind kind) {
    if (!get_instance().initialized_) {
        std::cerr << "Error: Log::init() must be called before logging!" << std::endl;
        return;
    }

    Log& instance = get_instance();
    const std::string timestamp = get_timestamp();
    instance.start_thread();
    {
        std::unique_lock guard(instance.queue_mutex_);
        instance.log_queue_.push(Entry{message, context, kind, timestamp});
    }
    instance.cv_.notify_one();
}
void Log::register_consumer(std::unique_ptr<LogConsumer>&& consumer) {
    std::unique_lock guard(get_instance().config_mutex_);
    get_instance().consumers_.emplace_back(std::move(consumer));
}

void Log::clear_consumers() {
    flush();
    std::lock_guard guard(get_instance().config_mutex_);
    get_instance().consumers_.clear();
}

void Log::report_filenames(const bool report) {
    std::lock_guard guard(get_instance().config_mutex_);
    get_instance().filenames_ = report;
}

void Log::report_functions(const bool report) {
    std::lock_guard guard(get_instance().config_mutex_);
    get_instance().functions_ = report;
}

void Log::set_verbosity(const Kind kind) { get_instance().verbosity_ = kind; }

Log::Kind Log::get_verbosity() { return get_instance().verbosity_; }

void Log::set_category_filter(const std::regex& filter) {
    std::unique_lock guard(get_instance().config_mutex_);
    get_instance().category_filter_ = std::make_unique<std::regex>(filter);
}

void Log::unset_category_filter() {
    std::unique_lock guard(get_instance().config_mutex_);
    get_instance().category_filter_.reset();
}

bool Log::has_category_filter() {
    std::unique_lock guard(get_instance().config_mutex_);
    return !!get_instance().category_filter_;
}

std::regex Log::get_category_filter() {
    std::unique_lock guard(get_instance().config_mutex_);
    return get_instance().category_filter_ ? *get_instance().category_filter_ : std::regex{};
}

void Log::flush() {
    std::unique_lock guard(get_instance().cv_mutex_);

    if (!get_instance().logging_ || !get_instance().logging_thread_.joinable()) {
        return;
    }

    get_instance().flush_requested_ = true;
    get_instance().cv_.notify_all();

    get_instance().cv_.wait(guard, [&] { return !get_instance().flush_requested_ && get_instance().log_queue_.empty(); });
}

std::string Log::get_timestamp() {
    const auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_buf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

void Log::start_thread() {
    std::unique_lock guard(cv_mutex_);
    if (!logging_) {
        logging_ = true;
        logging_thread_ = std::thread([this] { run(); });
    }
}

void Log::run() {
    std::unique_lock lock(cv_mutex_);
    while (logging_) {
        cv_.wait(lock, [this] { return !logging_ || !log_queue_.empty() || flush_requested_; });

        if (!logging_) {
            break;
        }

        std::queue<Entry> local_queue;
        {
            std::unique_lock queue_lock(queue_mutex_);
            std::swap(local_queue, log_queue_);
        }

        bool needs_flush = false;
        {
            std::lock_guard config_guard(config_mutex_);
            while (!local_queue.empty()) {

                if (Entry& entry = local_queue.front(); preprocess(entry)) {
                    for (const auto& consumer : consumers_) {
                        consumer->output_log(entry);
                    }
                }

                local_queue.pop();
            }
        }

        if (flush_requested_) {
            flush_requested_ = false;
            needs_flush = true;
        }

        if (needs_flush) {
            cv_.notify_all();
        }
    }
}

bool Log::preprocess(Entry& entry) const {
    if (category_filter_ && !regex_search(entry.context.category, *category_filter_)) {
        return false;
    }

    if (filenames_) {
        if (entry.context.filename) {
            const char* last_slash = nullptr;
            const char* last_backslash = nullptr;
            last_slash = strrchr(entry.context.filename, '/');
            last_backslash = strrchr(entry.context.filename, '\\');

            const char* filename_only = entry.context.filename;
            if (last_slash && last_backslash) {
                filename_only = last_slash > last_backslash ? last_slash + 1 : last_backslash + 1;
            } else if (last_slash) {
                filename_only = last_slash + 1;
            } else if (last_backslash) {
                filename_only = last_backslash + 1;
            }

            entry.context.filename = filename_only;
        }
    } else {
        entry.context.filename = nullptr;
    }

    if (!functions_) {
        entry.context.function = nullptr;
    }

    return true;
}
} // namespace common_utils::log
