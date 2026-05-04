#include "common-utils/StdoutConsumer.hpp"
#include <iostream>
namespace common_utils::log {
StdoutConsumer::StdoutConsumer(const Log::LogInfo& log_info) {
    std::cout << "===================================================================================\n";
    std::cout << "  Logging System Initialized\n";
    std::cout << "===================================================================================\n";
    std::cout << "  Application : " << log_info.app_name << "\n";
    std::cout << "  Verbosity   : " << log_info.verbosity << "\n";
    std::cout << "  File Logger : " << (log_info.use_log_file ? ": yes" : ": no") << "\n";
    std::cout << "===================================================================================\n\n";
    std::cout.flush();
}
void StdoutConsumer::output_log(const Log::Entry& entry) {
    std::ostream& stream = get_stream(entry);
    print_timestamp(stream, entry, true);
    print_header(stream, entry, true);
    print_message(stream, entry, true);
    print_context(stream, entry, true);
    print_new_line(stream, true);
    stream.flush();
}
std::ostream& StdoutConsumer::get_stream(const Log::Entry& entry) {
    (void)entry;
    return std::cout;
}
} // namespace common_utils::log
