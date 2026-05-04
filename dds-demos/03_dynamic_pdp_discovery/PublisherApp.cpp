// Copyright 2024 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file ClientPublisherApp.cpp
 *
 */

#include "PublisherApp.hpp"

#include <condition_variable>
#include <stdexcept>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.hpp>
#include <fastdds/rtps/transport/TCPv6TransportDescriptor.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.hpp>
#include <fastdds/rtps/transport/UDPv6TransportDescriptor.hpp>

#include "HelloWorldPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;

namespace eprosima {
namespace fastdds {
namespace examples {
namespace discovery_server {

PublisherApp::PublisherApp(const CLIParser::publisher_config& config) :
    participant_(nullptr), publisher_(nullptr), topic_(nullptr), writer_(nullptr), type_(new HelloWorldPubSubType()),
    matched_(0), samples_(config.samples), stop_(false), period_ms_(config.interval) {
    // Set up the data type with initial values
    hello_.index(0);
    hello_.message("Hello world");

    // Configure Participant QoS
    DomainParticipantQos pqos;
    pqos.name("DSS_SD_publisher");
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

    // Create service discovery locator
    rtps::Locator sd_locator;
    rtps::IPLocator::setPhysicalPort(sd_locator, server_port);

    std::shared_ptr<rtps::TransportDescriptorInterface> sd_tp;
    switch (config.transport_kind) {
    case TransportKind::UDPv4: {
        const auto descriptor_tmp = std::make_shared<rtps::UDPv4TransportDescriptor>();
        // descriptor_tmp->interfaceWhiteList.push_back(ip_server_address);
        sd_tp = descriptor_tmp;

        sd_locator.kind = LOCATOR_KIND_UDPv4;
        rtps::IPLocator::setIPv4(sd_locator, ip_server_address);
        break;
    }case TransportKind::UDPv6: {
        const auto descriptor_tmp = std::make_shared<rtps::UDPv6TransportDescriptor>();
        // descriptor_tmp->interfaceWhiteList.push_back(ip_server_address);
        sd_tp = descriptor_tmp;

        sd_locator.kind = LOCATOR_KIND_UDPv6;
        rtps::IPLocator::setIPv6(sd_locator, ip_server_address);
        break;
    }case TransportKind::TCPv4: {
        const auto descriptor_tmp = std::make_shared<rtps::TCPv4TransportDescriptor>();
        descriptor_tmp->add_listener_port(0);
        sd_tp = descriptor_tmp;

        sd_locator.kind = LOCATOR_KIND_TCPv4;
        rtps::IPLocator::setLogicalPort(sd_locator, server_port);
        rtps::IPLocator::setIPv4(sd_locator, ip_server_address);
        break;
    }case TransportKind::TCPv6: {
        const auto descriptor_tmp = std::make_shared<rtps::TCPv6TransportDescriptor>();
        descriptor_tmp->add_listener_port(0);
        sd_tp = descriptor_tmp;

        sd_locator.kind = LOCATOR_KIND_TCPv6;
        rtps::IPLocator::setLogicalPort(sd_locator, server_port);
        rtps::IPLocator::setIPv6(sd_locator, ip_server_address);
        break;
    }default:
        break;
    }
    // Set participant as SD CLIENT
    pqos.wire_protocol().builtin.discovery_config.discoveryProtocol = rtps::DiscoveryProtocol::CLIENT;
    // Add server discovery locator
    pqos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(sd_locator);

    // Add descriptor
    pqos.transport().user_transports.push_back(sd_tp);

    // Create Domain participant
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos, nullptr);

    if (participant_ == nullptr) {
        throw std::runtime_error("Participant initialization failed");
    }
    std::cout << "Publisher Participant " << pqos.name() << " created with GUID " << participant_->guid()
              << " connecting to PDP server <" << sd_locator << "> " << std::endl;

    // Regsiter type
    type_.register_type(participant_);

    // Create the publisher
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
    if (publisher_ == nullptr) {
        throw std::runtime_error("Publisher initialization failed");
    }

    // Create the topic
    topic_ = participant_->create_topic(config.topic_name, type_.get_type_name(), TOPIC_QOS_DEFAULT);
    if (topic_ == nullptr) {
        throw std::runtime_error("Topic initialization failed");
    }

    // Create de data writer
    DataWriterQos writer_qos = DATAWRITER_QOS_DEFAULT;
    if (!config.reliable) {
        writer_qos.reliability().kind = BEST_EFFORT_RELIABILITY_QOS;
    }
    if (!config.transient_local) {
        writer_qos.durability().kind = VOLATILE_DURABILITY_QOS;
    }
    // So as not to overwriter the first sample
    // if we publish immediately after the discovery,and the subscription is not prepared yet
    writer_qos.history().depth = 5;
    writer_ = publisher_->create_datawriter(topic_, writer_qos, this);
    if (writer_ == nullptr) {
        throw std::runtime_error("DataWriter initialization failed");
    }
}

PublisherApp::~PublisherApp() {
    if (nullptr != participant_) {
        // Delete DDS entities contained within the DomainParticipant
        participant_->delete_contained_entities();

        // Delete DomainParticipant
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
}

void PublisherApp::on_publication_matched(DataWriter* /*writer*/, const PublicationMatchedStatus& info) {
    if (info.current_count_change == 1) {
        matched_ = static_cast<int16_t>(info.current_count);
        std::cout << "Publisher matched." << std::endl;
        cv_.notify_one();
    } else if (info.current_count_change == -1) {
        matched_ = static_cast<int16_t>(info.current_count);
        std::cout << "Publisher unmatched." << std::endl;
    } else {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
}

void PublisherApp::run() {
    while (!is_stopped() && ((samples_ == 0) || (hello_.index() < samples_))) {
        if (publish()) {
            std::cout << "Message: '" << hello_.message() << "' with index: '" << hello_.index() << "' SENT"
                      << std::endl;

            if (hello_.index() == 1u) {
                ReturnCode_t acked = RETCODE_ERROR;
                do {
                    Duration_t acked_wait{1, 0};
                    acked = writer_->wait_for_acknowledgments(acked_wait);
                } while (acked != RETCODE_OK);
            }
        }
        // Wait for period or stop event
        std::unique_lock<std::mutex> period_lock(mutex_);
        cv_.wait_for(period_lock, std::chrono::milliseconds(period_ms_), [&]() { return is_stopped(); });
    }
}

bool PublisherApp::publish() {
    bool ret = false;
    // Wait for the data endpoints discovery
    std::unique_lock<std::mutex> matched_lock(mutex_);
    cv_.wait(matched_lock, [&]() {
        // at least one has been discovered
        return matched_ > 0 || is_stopped();
    });

    if (!is_stopped()) {
        hello_.index(hello_.index() + 1);
        ret = RETCODE_OK == writer_->write(&hello_);
    }
    return ret;
}

bool PublisherApp::is_stopped()const { return stop_.load(); }

void PublisherApp::stop() {
    stop_.store(true);
    cv_.notify_one();
}

} // namespace discovery_server
} // namespace examples
} // namespace fastdds
} // namespace eprosima
