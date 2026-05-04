#include <common-utils/LogConsumer.hpp>
namespace common_utils::log {
void LogConsumer::print_timestamp(std::ostream& stream, const Log::Entry& entry, const bool color) {
    const std::string white = color ? C_B_WHITE : "";
    stream << white << entry.timestamp << " ";
}

void LogConsumer::print_header(std::ostream& stream, const Log::Entry& entry, const bool color) {
    const std::string c_b_color = !color   ? ""
        : entry.kind == Log::Kind::Error   ? C_B_RED
        : entry.kind == Log::Kind::Warning ? C_B_YELLOW
        : entry.kind == Log::Kind::Info    ? C_B_GREEN
        : entry.kind == Log::Kind::Debug   ? C_GRAY
                                           : "";

    const std::string white = color ? C_B_WHITE : "";

    stream << c_b_color << "[" << white << entry.context.category << c_b_color << " " << entry.kind << "] ";
}

void LogConsumer::print_context(std::ostream& stream, const Log::Entry& entry, const bool color) {
    if (color) {
        stream << C_B_BLUE;
    }
    if (entry.context.filename) {
        stream<< " - " << entry.context.filename;
        stream << ":" << entry.context.line;
    }
    if (entry.context.function) {
        stream << " : ";
        if (color) {
            stream << C_CYAN;
        }
        stream << entry.context.function;
    }
}

void LogConsumer::print_message(std::ostream& stream, const Log::Entry& entry, const bool color) {
    const std::string white = color ? C_WHITE : "";
    stream << white << entry.message;
}

void LogConsumer::print_new_line(std::ostream& stream, const bool color) {
    const std::string def = color ? C_DEF : "";
    stream << def << std::endl;
}
} // namespace common_utils::log