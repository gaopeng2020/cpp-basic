
/**
 * @file main.cpp
 *
 */

#include <csignal>
#include <stdexcept>
#include <thread>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/log/Log.hpp>

#include "Application.hpp"
#include "CLIParser.hpp"

using eprosima::fastdds::dds::Log;

using namespace eprosima::fastdds::examples::discovery_server;

std::function<void(int)> stop_app_handler;
void signal_handler(int signum) { stop_app_handler(signum); }

int main(int argc, char** argv) {
    auto ret = EXIT_SUCCESS;
    CLIParser::ds_example_config config = CLIParser::parse_cli_options(argc, argv);
    uint16_t samples = 0;
    switch (config.entity) {
    case CLIParser::EntityKind::PUBLISHER:
        samples = config.pub_config.samples;
        break;
    case CLIParser::EntityKind::SUBSCRIBER:
        samples = config.sub_config.samples;
        break;
    default:
        break;
    }

    std::string app_name = CLIParser::parse_entity_kind(config.entity);
    std::shared_ptr<Application> app;

    try {
        app = Application::make_app(config);
    } catch (const std::runtime_error& e) {
        EPROSIMA_LOG_ERROR(app_name, e.what());
        ret = EXIT_FAILURE;
    }

    if (EXIT_FAILURE != ret) {
        std::thread thread(&Application::run, app);

        if (samples == 0) {
            std::cout << app_name << " running. Please press Ctrl+C to stop the " << app_name << " at any time."
                      << std::endl;
        } else {
            std::cout << app_name << " running for " << samples << " samples. Please press Ctrl+C to stop the "
                      << app_name << " at any time." << std::endl;
        }

        stop_app_handler = [&](int signum) {
            std::cout << "\n"
                      << CLIParser::parse_signal(signum) << " received, stopping " << app_name << " execution."
                      << std::endl;
            app->stop();
        };

        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
#ifndef _WIN32
        signal(SIGQUIT, signal_handler);
        signal(SIGHUP, signal_handler);
#endif // _WIN32

        thread.join();
    }

    Log::Reset();
    return ret;
}
