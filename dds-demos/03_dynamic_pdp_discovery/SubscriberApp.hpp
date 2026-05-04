
/**
 * @file ClientSubscriberApp.hpp
 *
 */

#ifndef FASTDDS_EXAMPLES_CPP_DISCOVERY_SERVER__CLIENTSUBSCRIBERAPP_HPP
#define FASTDDS_EXAMPLES_CPP_DISCOVERY_SERVER__CLIENTSUBSCRIBERAPP_HPP

#include <condition_variable>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include "Application.hpp"
#include "CLIParser.hpp"
#include "HelloWorld.hpp"

using namespace eprosima::fastdds::dds;

namespace eprosima {
namespace fastdds {
namespace examples {
namespace discovery_server {

class SubscriberApp : public Application, public DataReaderListener {
public:
    SubscriberApp(const CLIParser::subscriber_config& config);

    ~SubscriberApp();

    //! Subscription callback
    void on_data_available(DataReader* reader) override;

    //! Subscriber matched method
    void on_subscription_matched(DataReader* reader, const SubscriptionMatchedStatus& info) override;

    //! Run subscriber
    void run() override;

    //! Trigger the end of execution
    void stop() override;

private:
    //! Return the current state of execution
    bool is_stopped() const;

    HelloWorld hello_;

    DomainParticipant* participant_;

    Subscriber* subscriber_;

    Topic* topic_;

    DataReader* reader_;

    TypeSupport type_;

    uint16_t samples_;

    uint16_t received_samples_;

    std::atomic<bool> stop_;

    mutable std::mutex terminate_cv_mtx_;

    std::condition_variable terminate_cv_;
};

} // namespace discovery_server
} // namespace examples
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_EXAMPLES_CPP_DISCOVERY_SERVER__CLIENTSUBSCRIBERAPP_HPP
