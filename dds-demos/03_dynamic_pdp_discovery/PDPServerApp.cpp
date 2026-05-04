/**
 * @file ServerApp.cpp
 *
 */

#include "PDPServerApp.hpp"

#include <condition_variable>
#include <stdexcept>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.hpp>
#include <fastdds/rtps/transport/TCPv6TransportDescriptor.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.hpp>
#include <fastdds/rtps/transport/UDPv6TransportDescriptor.hpp>

using namespace eprosima::fastdds::dds;

namespace eprosima {
namespace fastdds {
namespace examples {
namespace discovery_server {

PDPServerApp::PDPServerApp(const CLIParser::pdp_server_config& config) :
    participant_(nullptr), matched_(0), timeout_(config.timeout), start_time_(std::chrono::steady_clock::now()),
    stop_(false) {
    DomainParticipantQos pqos;
    pqos.name("PDP-Server");
    pqos.transport().use_builtin_transports = false;

    std::string ip_listening_address(config.listening_address);
    std::string ip_connection_address(config.connection_address);
    // Check if DNS is required
    if (!is_ip(config.listening_address)) {
        ip_listening_address = get_ip_from_dns(config.listening_address, config.transport_kind);
    }

    if (ip_listening_address.empty()) {
        throw std::runtime_error("Invalid listening address");
    }

    // Do the same for connection
    if (config.is_also_client && !is_ip(config.connection_address)) {
        ip_connection_address = get_ip_from_dns(config.connection_address, config.transport_kind);
    }

    if (config.is_also_client && ip_connection_address.empty()) {
        throw std::runtime_error("Invalid connection address");
    }

    // Configure Listening address
    // Create DS SERVER locator
    rtps::Locator listening_locator,connection_locator;
    rtps::IPLocator::setPhysicalPort(listening_locator, config.listening_port);
    //as a client to connect pdp server
    rtps::IPLocator::setPhysicalPort(connection_locator, config.connection_port);

    std::shared_ptr<rtps::TransportDescriptorInterface> descriptor;

    switch (config.transport_kind) {
    case TransportKind::UDPv4: {
        const auto descriptor_tmp = std::make_shared<rtps::UDPv4TransportDescriptor>();
        descriptor = descriptor_tmp;

        listening_locator.kind = LOCATOR_KIND_UDPv4;
        rtps::IPLocator::setIPv4(listening_locator, ip_listening_address);
        connection_locator.kind = LOCATOR_KIND_UDPv4;
        rtps::IPLocator::setIPv4(connection_locator, ip_connection_address);
        break;
    } case TransportKind::UDPv6: {
        const auto descriptor_tmp = std::make_shared<rtps::UDPv6TransportDescriptor>();
        descriptor = descriptor_tmp;

        listening_locator.kind = LOCATOR_KIND_UDPv6;
        rtps::IPLocator::setIPv6(listening_locator, ip_listening_address);
        connection_locator.kind = LOCATOR_KIND_UDPv6;
        rtps::IPLocator::setIPv6(connection_locator, ip_connection_address);
        break;
    } case TransportKind::TCPv4: {
        const auto descriptor_tmp = std::make_shared<rtps::TCPv4TransportDescriptor>();
        descriptor_tmp->add_listener_port(config.listening_port);
        descriptor = descriptor_tmp;

        listening_locator.kind = LOCATOR_KIND_TCPv4;
        rtps::IPLocator::setLogicalPort(listening_locator, config.listening_port);
        rtps::IPLocator::setIPv4(listening_locator, ip_listening_address);
        connection_locator.kind = LOCATOR_KIND_TCPv4;
        rtps::IPLocator::setIPv4(connection_locator, ip_connection_address);
        rtps::IPLocator::setLogicalPort(connection_locator, config.connection_port);
        break;
    } case TransportKind::TCPv6: {
       const auto descriptor_tmp = std::make_shared<rtps::TCPv6TransportDescriptor>();
        descriptor_tmp->add_listener_port(config.listening_port);
        descriptor = descriptor_tmp;

        listening_locator.kind = LOCATOR_KIND_TCPv6;
        rtps::IPLocator::setLogicalPort(listening_locator, config.listening_port);
        rtps::IPLocator::setIPv6(listening_locator, ip_listening_address);
        connection_locator.kind = LOCATOR_KIND_TCPv6;
        rtps::IPLocator::setIPv6(connection_locator, ip_connection_address);
        rtps::IPLocator::setLogicalPort(connection_locator, config.connection_port);
        break;
    } default:
        break;
    }

    // Add descriptor
    pqos.transport().user_transports.push_back(descriptor);

    // Set participant as SERVER
    pqos.wire_protocol().builtin.discovery_config.discoveryProtocol = rtps::DiscoveryProtocol::SERVER;

    // Set SERVER's listening locator for PDP
    pqos.wire_protocol().builtin.metatrafficUnicastLocatorList.push_back(listening_locator);

    // Configure Connection address
    if (config.is_also_client) {
        // Add remote SERVER to CLIENT's list of SERVERs
        pqos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(connection_locator);
    }


    // Create Participant
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos, this);

    if (participant_ == nullptr) {
        throw std::runtime_error("Participant initialization failed");
    }

    if (config.is_also_client) {
        std::cout << " PDP server Participant:" << pqos.name() << " created with GUID " << participant_->guid()
                  << " listening in address <" << listening_locator << "> "
                  << " connected to address <" << connection_locator << "> " << std::endl;
    } else {
        std::cout << "Server Participant " << pqos.name() << " created with GUID " << participant_->guid()
                  << " listening in address <" << listening_locator << "> " << std::endl;
    }
}

PDPServerApp::~PDPServerApp() {
    if (nullptr != participant_) {
        // Delete DDS entities contained within the DomainParticipant
        participant_->delete_contained_entities();

        // Delete DomainParticipant
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
}

void PDPServerApp::on_participant_discovery(DomainParticipant*, const fastdds::rtps::ParticipantDiscoveryStatus status,
                                         const ParticipantBuiltinTopicData& info, bool& should_be_ignored) {
    static_cast<void>(should_be_ignored);
    if (status == rtps::ParticipantDiscoveryStatus::DISCOVERED_PARTICIPANT) {
        std::cout << "Discovered Participant with GUID " << info.guid << std::endl;
        ++matched_;
    } else if (status == rtps::ParticipantDiscoveryStatus::DROPPED_PARTICIPANT ||
               status == rtps::ParticipantDiscoveryStatus::REMOVED_PARTICIPANT) {
        std::cout << "Dropped Participant with GUID " << info.guid << std::endl;
        --matched_;
    }
}

void PDPServerApp::run() {
    while (!is_stopped()) {
        // Wait for period or stop event
        std::unique_lock<std::mutex> period_lock(mutex_);

        if (timeout_ != 0) {
            bool timeout = false;
            cv_.wait_for(period_lock, std::chrono::seconds(timeout_), [&]() {
                timeout =
                    ((std::chrono::steady_clock::now() - start_time_) >= std::chrono::milliseconds(timeout_ * 1000));
                return is_stopped() || timeout;
            });

            if (timeout) {
                stop();
            }
        } else {
            cv_.wait(period_lock, [&]() { return is_stopped(); });
        }
    }
}

bool PDPServerApp::is_stopped() const { return stop_.load(); }

void PDPServerApp::stop() {
    stop_.store(true);
    cv_.notify_one();
}

} // namespace discovery_server
} // namespace examples
} // namespace fastdds
} // namespace eprosima
