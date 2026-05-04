
/**
 * @file ClientSubscriberApp.cpp
 *
 */

#include "SubscriberApp.hpp"

#include <condition_variable>
#include <stdexcept>

#include <fastdds/dds/core/status/SubscriptionMatchedStatus.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/qos/SubscriberQos.hpp>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.hpp>
#include <fastdds/rtps/transport/TCPv6TransportDescriptor.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.hpp>
#include <fastdds/rtps/transport/UDPv6TransportDescriptor.hpp>

#include "Application.hpp"
#include "CLIParser.hpp"

#include "HelloWorldPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;

namespace eprosima {
namespace fastdds {
namespace examples {
namespace discovery_server {

SubscriberApp::SubscriberApp(const CLIParser::subscriber_config& config) :
    participant_(nullptr), subscriber_(nullptr), topic_(nullptr), reader_(nullptr), type_(new HelloWorldPubSubType()),
    samples_(config.samples), received_samples_(0), stop_(false) {
    DomainParticipantQos pqos;
    pqos.name("DDS-SD_Subscriber");
    pqos.transport().use_builtin_transports = false;

   const uint16_t server_port = config.connection_port;

    std::string ip_server_address(config.connection_address);
    // Check if DNS is required
    if (!is_ip(config.connection_address)) {
        ip_server_address = get_ip_from_dns(config.connection_address, config.transport_kind);
    }

    if (ip_server_address.empty()) {
        throw std::runtime_error("Invalid connection address");
    }

    // Create DS SERVER locator
    rtps::Locator sd_locator;
    rtps::IPLocator::setPhysicalPort(sd_locator, server_port);

    std::shared_ptr<rtps::TransportDescriptorInterface> tp_sd;

    switch (config.transport_kind) {
    case TransportKind::UDPv4: {
        const auto descriptor_tmp = std::make_shared<rtps::UDPv4TransportDescriptor>();
        // descriptor_tmp->interfaceWhiteList.push_back(ip_server_address);
        tp_sd = descriptor_tmp;

        sd_locator.kind = LOCATOR_KIND_UDPv4;
        rtps::IPLocator::setIPv4(sd_locator, ip_server_address);
        break;
    } case TransportKind::UDPv6: {
      const auto descriptor_tmp = std::make_shared<rtps::UDPv6TransportDescriptor>();
        // descriptor_tmp->interfaceWhiteList.push_back(ip_server_address);
        tp_sd = descriptor_tmp;

        sd_locator.kind = LOCATOR_KIND_UDPv6;
        rtps::IPLocator::setIPv6(sd_locator, ip_server_address);
        break;
    }  case TransportKind::TCPv4: {
       const auto descriptor_tmp = std::make_shared<rtps::TCPv4TransportDescriptor>();
        // descriptor_tmp->interfaceWhiteList.push_back(ip_server_address);
        // One listening port must be added either in the pub or the sub
        descriptor_tmp->add_listener_port(0);
        tp_sd = descriptor_tmp;

        sd_locator.kind = LOCATOR_KIND_TCPv4;
        rtps::IPLocator::setLogicalPort(sd_locator, server_port);
        rtps::IPLocator::setIPv4(sd_locator, ip_server_address);
        break;
    } case TransportKind::TCPv6: {
       const auto descriptor_tmp = std::make_shared<rtps::TCPv6TransportDescriptor>();
        // descriptor_tmp->interfaceWhiteList.push_back(ip_server_address);
        // One listening port must be added either in the pub or the sub
        descriptor_tmp->add_listener_port(0);
        tp_sd = descriptor_tmp;

        sd_locator.kind = LOCATOR_KIND_TCPv6;
        rtps::IPLocator::setLogicalPort(sd_locator, server_port);
        rtps::IPLocator::setIPv6(sd_locator, ip_server_address);
        break;
    } default:
        break;
    }

    // Set participant as DS CLIENT
    pqos.wire_protocol().builtin.discovery_config.discoveryProtocol =  rtps::DiscoveryProtocol::CLIENT;

    // Add remote SERVER to CLIENT's list of SERVERs
    pqos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(sd_locator);

    // Add descriptor
    pqos.transport().user_transports.push_back(tp_sd);

    // Create the Domain participant
    participant_ = DomainParticipantFactory::get_instance()->create_participant(
        0, pqos, nullptr, StatusMask::all() >> StatusMask::data_on_readers());

    if (participant_ == nullptr) {
        throw std::runtime_error("Participant initialization failed");
    }

    std::cout << "Subscriber Participant " << pqos.name() << " created with GUID " << participant_->guid()
              << " connecting to server <" << sd_locator << "> " << std::endl;

    // Register the type
    type_.register_type(participant_);

    // Create the subscriber
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
    if (subscriber_ == nullptr) {
        throw std::runtime_error("Subscriber initialization failed");
    }

    // Create the topic
    topic_ = participant_->create_topic(config.topic_name, type_.get_type_name(), TOPIC_QOS_DEFAULT);
    if (topic_ == nullptr) {
        throw std::runtime_error("Topic initialization failed");
    }

    // Create the data reader
    DataReaderQos reader_qos = DATAREADER_QOS_DEFAULT;
    if (config.reliable) {
        reader_qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    }
    if (config.transient_local) {
        reader_qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
    }
    reader_ = subscriber_->create_datareader(topic_, reader_qos, this);
    if (reader_ == nullptr) {
        throw std::runtime_error("DataWriter initialization failed");
    }
}

SubscriberApp::~SubscriberApp() {
    if (nullptr != participant_) {
        // Delete DDS entities contained within the DomainParticipant
        participant_->delete_contained_entities();

        // Delete DomainParticipant
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
}

void SubscriberApp::on_subscription_matched(DataReader* /*reader*/, const SubscriptionMatchedStatus& info) {
    if (info.current_count_change == 1) {
        std::cout << "Subscriber matched." << std::endl;
    } else if (info.current_count_change == -1) {
        std::cout << "Subscriber unmatched." << std::endl;
    } else {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
}

void SubscriberApp::on_data_available(DataReader* reader) {
    SampleInfo info;
    while ((!is_stopped()) && (RETCODE_OK == reader->take_next_sample(&hello_, &info))) {
        if ((info.instance_state == ALIVE_INSTANCE_STATE) && info.valid_data) {
            received_samples_++;
            // Print Hello world message data
            std::cout << "Message: '" << hello_.message() << "' with index: '" << hello_.index() << "' RECEIVED"
                      << std::endl;
            if (samples_ > 0 && (received_samples_ >= samples_)) {
                stop();
            }
        }
    }
}

void SubscriberApp::run() {
    std::unique_lock<std::mutex> lck(terminate_cv_mtx_);
    terminate_cv_.wait(lck, [&] { return is_stopped(); });
}

bool SubscriberApp::is_stopped() const { return stop_.load(); }

void SubscriberApp::stop() {
    stop_.store(true);
    terminate_cv_.notify_all();
}

} // namespace discovery_server
} // namespace examples
} // namespace fastdds
} // namespace eprosima
