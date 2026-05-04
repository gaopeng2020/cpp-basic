/**
 * @file ServerApp.hpp
 *
 */

#ifndef FASTDDS_EXAMPLES_CPP_DISCOVERY_SERVER__SERVERAPP_HPP
#define FASTDDS_EXAMPLES_CPP_DISCOVERY_SERVER__SERVERAPP_HPP

#include <condition_variable>

#include <fastdds/dds/builtin/topic/ParticipantBuiltinTopicData.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include "Application.hpp"
#include "CLIParser.hpp"
#include "HelloWorldPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;

namespace eprosima {
namespace fastdds {
namespace examples {
namespace discovery_server {

class PDPServerApp : public Application, public DomainParticipantListener {
public:
    explicit PDPServerApp(const CLIParser::pdp_server_config& config);

    ~PDPServerApp() override;

    //! Publisher matched method
    void on_participant_discovery(DomainParticipant* participant, rtps::ParticipantDiscoveryStatus status,
                                  const rtps::ParticipantBuiltinTopicData& info,
                                  bool& should_be_ignored) override;

    //! Run publisher
    void run() override;

    //! Stop publisher
    void stop() override;

private:
    //! Return the current state of execution
    bool is_stopped() const;
    std::atomic<bool> stop_;

    DomainParticipant* participant_;

    int16_t matched_;
    std::mutex mutex_;
    uint16_t timeout_;
    std::chrono::steady_clock::time_point start_time_;
    std::condition_variable cv_;

};

} // namespace discovery_server
} // namespace examples
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_EXAMPLES_CPP_DISCOVERY_SERVER__SERVERAPP_HPP
