#include "Application.hpp"

#include "CLIParser.hpp"
#include "PublisherApp.hpp"
#include "SubscriberApp.hpp"

using namespace eprosima::fastdds::dds;

namespace eprosima {
namespace fastdds {
namespace examples {
namespace static_edp_discovery {

//! Factory method to create a publisher or subscriber
std::shared_ptr<Application> Application::make_app(const CLIParser::static_edp_discovery_config& config,
                                                   const std::string& topic_name) {
    std::shared_ptr<Application> entity;
    switch (config.entity) {
    case CLIParser::EntityKind::PUBLISHER:
        entity = std::make_shared<PublisherApp>(config, topic_name);
        break;
    case CLIParser::EntityKind::SUBSCRIBER:
        entity = std::make_shared<SubscriberApp>(config, topic_name);
        break;
    case CLIParser::EntityKind::UNDEFINED:
    default:
        throw std::runtime_error("Entity initialization failed");
        break;
    }
    return entity;
}

} // namespace static_edp_discovery
} // namespace examples
} // namespace fastdds
} // namespace eprosima
