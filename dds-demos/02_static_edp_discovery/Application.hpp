/**
 * @file Application.hpp
 *
 */

#ifndef FASTDDS_EXAMPLES_CPP_STATIC_EDP_DISCOVERY__APPLICATION_HPP
#define FASTDDS_EXAMPLES_CPP_STATIC_EDP_DISCOVERY__APPLICATION_HPP

#include <atomic>

#include "CLIParser.hpp"

namespace eprosima {
namespace fastdds {
namespace examples {
namespace static_edp_discovery {

class Application {
public:
    //! Virtual destructor
    virtual ~Application() = default;

    //! Run application
    virtual void run() = 0;

    //! Trigger the end of execution
    virtual void stop() = 0;

    //! Factory method to create applications based on configuration
    static std::shared_ptr<Application> make_app(const CLIParser::static_edp_discovery_config& config, const std::string& topic_name);
};

} // namespace static_edp_discovery
} // namespace examples
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_EXAMPLES_CPP_STATIC_EDP_DISCOVERY__APPLICATION_HPP
