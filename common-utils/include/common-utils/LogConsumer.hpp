#pragma once

#include <common-utils/Log.hpp>
namespace common_utils::log {
// 定义日志颜色
#define C_DEF "\033[m"          // 默认颜色（重置）
#define C_RED "\033[31m"        // 红色
#define C_B_RED "\033[31;1m"    // 亮红色（粗体）
#define C_GREEN "\033[32m"      // 绿色
#define C_B_GREEN "\033[32;1m"  // 亮绿色（粗体）
#define C_YELLOW "\033[33m"     // 黄色
#define C_B_YELLOW "\033[33;1m" // 亮黄色（粗体）
#define C_BLUE "\033[34m"       // 蓝色
#define C_B_BLUE "\033[34;1m"   // 亮蓝色（粗体）
#define C_MAGENTA "\033[35m"    // 洋红色
#define C_B_MAGENTA "\033[35;1m" // 亮洋红色（粗体）
#define C_CYAN "\033[36m"       // 青色
#define C_B_CYAN "\033[36;1m"   // 亮青色（粗体）
#define C_WHITE "\033[37m"      // 白色
#define C_B_WHITE "\033[37;1m"  // 亮白色（粗体）
#define C_GRAY "\033[90m"       // 灰色
#define C_BRIGHT "\033[1m"      // 高亮（粗体）


/**
 * @class LogConsumer
 * @brief Base class for log output consumers.
 *
 * Defines the interface for consuming log entries and provides helper methods
 * for formatting log output with optional color support.
 */
class LogConsumer {
public:
    virtual ~LogConsumer() = default;

    /**
     * @brief Process and output a log entry.
     * @param entry The log entry to consume.
     */
    virtual void output_log(const Log::Entry& entry) = 0;

protected:
    /**
     * @brief Get the output stream for the log entry.
     * @param entry The log entry being processed.
     * @return Reference to the target output stream.
     */
    virtual std::ostream& get_stream(const Log::Entry& entry) = 0;

    /**
     * @brief Print the timestamp of the log entry.
     * @param stream Output stream to write to.
     * @param entry The log entry containing timestamp.
     * @param color Enable colored output if true.
     */
    static void print_timestamp(std::ostream& stream, const Log::Entry& entry, bool color);

    /**
     * @brief Print the log header (category and severity level).
     * @param stream Output stream to write to.
     * @param entry The log entry containing metadata.
     * @param color Enable colored output if true.
     */
    static void print_header(std::ostream& stream, const Log::Entry& entry, bool color);

    /**
     * @brief Print source context (filename, line, function).
     * @param stream Output stream to write to.
     * @param entry The log entry containing context information.
     * @param color Enable colored output if true.
     */
    static void print_context(std::ostream& stream, const Log::Entry& entry, bool color);

    /**
     * @brief Print the log message content.
     * @param stream Output stream to write to.
     * @param entry The log entry containing the message.
     * @param color Enable colored output if true.
     */
    static void print_message(std::ostream& stream, const Log::Entry& entry, bool color);

    /**
     * @brief Print a newline and reset color formatting.
     * @param stream Output stream to write to.
     * @param color Reset color if true.
     */
    static void print_new_line(std::ostream& stream, bool color);
};
} // namespace common_utils::log