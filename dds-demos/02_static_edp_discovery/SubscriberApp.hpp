
/**
 * @file SubscriberApp.hpp
 *
 */

#ifndef FASTDDS_EXAMPLE_CPP_STATIC_EDP_DISCOVERY__SUBSCRIBERAPP_HPP
#define FASTDDS_EXAMPLE_CPP_STATIC_EDP_DISCOVERY__SUBSCRIBERAPP_HPP

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
namespace static_edp_discovery {

class SubscriberApp : public Application, public DataReaderListener {
public:
    SubscriberApp(const CLIParser::static_edp_discovery_config& config, const std::string& topic_name);

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

    std::string xml_file_name;
};

} // namespace static_edp_discovery
} // namespace examples
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_EXAMPLE_CPP_STATIC_EDP_DISCOVERY__SUBSCRIBERAPP_HPP
