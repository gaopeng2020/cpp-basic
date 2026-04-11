#pragma once

#include <common-utils/LogConsumer.hpp>
namespace common_utils::log {
/**
 * @class StdoutConsumer
 * @brief Log consumer that outputs log events to standard output (stdout).
 */
class StdoutConsumer : public LogConsumer {
public:
  /**
   * @brief Constructor.
   * @param log_info Log configuration information including app name and
   * verbosity.
   */
  explicit StdoutConsumer(const Log::LogInfo &log_info);

  ~StdoutConsumer() override = default;

  /**
   * @brief Output a log entry to stdout.
   * @param entry The log entry to output.
   */
  void output_log(const Log::Entry &entry) override;

protected:
  /**
   * @brief Get the output stream for the log entry.
   * @param entry The log entry being processed.
   * @return Reference to std::cout.
   */
  std::ostream &get_stream(const Log::Entry &entry) override;

private:
  StdoutConsumer() = delete;
};
} // namespace common_utils::log