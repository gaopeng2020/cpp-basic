
/**
 * @file CLIParser.hpp
 *
 */
#ifndef FASTDDS_EXAMPLES_CPP_DISCOVERY_SERVER__CLIPARSER_HPP
#define FASTDDS_EXAMPLES_CPP_DISCOVERY_SERVER__CLIPARSER_HPP

#include <csignal>
#include <cstdlib>
#include <fastdds/dds/log/Log.hpp>
#include <iostream>
#include "Helpers.hpp"


namespace eprosima {
namespace fastdds {
namespace examples {
namespace discovery_server {

using dds::Log;

class CLIParser {
public:
    CLIParser() = delete;

    //! Entity kind enumeration
    enum class EntityKind : uint8_t { PUBLISHER, SUBSCRIBER, SD_SERVER, UNDEFINED };

    //! Clients common configuration
    struct pdp_client_config {
        uint16_t connection_port{16166};
        std::string connection_address{"127.0.0.1"};
    };

    //! Subscriber configuration structure
    struct subscriber_config : pdp_client_config {
        bool reliable{false};
        bool transient_local{false};
        uint16_t samples{0};
        std::string topic_name{"discovery_server_topic"};
        TransportKind transport_kind{TransportKind::UDPv4};
    };

    //! Publisher configuration structure
    struct publisher_config : subscriber_config {
        uint16_t interval{100};
    };

    //! Server configuration structure
    //! A server can, in turn, act as a client
    struct pdp_server_config : pdp_client_config {
        bool is_also_client{false};
        TransportKind transport_kind{TransportKind::UDPv4};
        uint16_t listening_port{16166};
        uint16_t timeout{0};
        std::string listening_address{"127.0.0.1"};
    };

    //! Configuration structure for the example
    struct ds_example_config {
        EntityKind entity = EntityKind::UNDEFINED;
        publisher_config pub_config;
        subscriber_config sub_config;
        pdp_server_config srv_config;

        // print the ds_example_config
        friend std::ostream& operator<<(std::ostream& os, const ds_example_config& config) {
            std::string tp_kind;
            switch (config.pub_config.transport_kind) {
            case TransportKind::UDPv4: {
                tp_kind = "UDPv4";
                break;
            }
            case TransportKind::UDPv6: {
                tp_kind = "UDPv6";
                break;
            }
            case TransportKind::TCPv4: {
                tp_kind = "TCPv4";
                break;
            }
            case TransportKind::TCPv6: {
                tp_kind = "TCPv6";
                break;
            }
            default:
                break;
            }
            os << "Entity: " << parse_entity_kind(config.entity) << std::endl;
            os << "  Transport type: " << tp_kind << std::endl;

            if ((config.entity == EntityKind::SD_SERVER && config.srv_config.is_also_client) ||
                config.entity != EntityKind::SD_SERVER) {
                os << "PDP Client options:" << std::endl;
                os << "  Connection address: " << config.pub_config.connection_address << std::endl;
                os << "  Connection port: " << config.pub_config.connection_port << std::endl;
            }

            if (config.entity == EntityKind::PUBLISHER) {
                os << "Publisher options:" << std::endl;
                os << "  Topic name: " << config.pub_config.topic_name << std::endl;
                os << "  Samples: " << config.pub_config.samples << std::endl;
                os << "  Interval: " << config.pub_config.interval << std::endl;
                os << "  is reliable: " << config.pub_config.reliable << std::endl;
                os << "  transient_local: " << config.pub_config.transient_local << std::endl;
            } else if (config.entity == EntityKind::SUBSCRIBER) {
                os << "Subscriber options:" << std::endl;
                os << "  Topic name: " << config.sub_config.topic_name << std::endl;
                os << "  Samples: " << config.sub_config.samples << std::endl;
                os << "  is reliable: " << config.sub_config.reliable << std::endl;
                os << "  transient_local: " << config.sub_config.transient_local << std::endl;
            } else if (config.entity == EntityKind::SD_SERVER) {
                os << "PDP Server options:" << std::endl;
                os << "  Listening address: " << config.srv_config.listening_address << std::endl;
                os << "  Listening port: " << config.srv_config.listening_port << std::endl;
                os << "  Timeout: " << config.srv_config.timeout << std::endl;
            }
            return os;
        }
    };

    /**
     * @brief Print usage help message and exit with the given return code
     *
     * @param return_code return code to exit with
     *
     * @warning This method finishes the execution of the program with the input return code
     */
    static void print_help(const uint8_t return_code) {
        std::cout << "Usage: dynamic_pdp_discovery <entity> [options]" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Entities:" << std::endl;
        std::cout << "  publisher                            Run a publisher entity." << std::endl;
        std::cout << "  subscriber                           Run a subscriber entity." << std::endl;
        std::cout << "  pdp_server                               Run a server entity." << std::endl;
        std::cout << "" << std::endl;
        std::cout << "  -h,       --help                     Print this help message." << std::endl;
        std::cout << "Common options (for Publisher, Subscriber and SD-Server acting as Client):" << std::endl;
        std::cout << "  -c <str>, --connection-address <str> Address of the Server to connect to" << std::endl;
        std::cout << "                                       (Default address: 127.0.0.1)." << std::endl;
        std::cout << "  -p <num>, --connection-port <num>    Port of the Server to connect to" << std::endl;
        std::cout << "                                       (Default port: 16166)." << std::endl;
        std::cout << "                                       (0 by default)." << std::endl;
        std::cout << "            --transport <str>          [udpv4|udpv6|tcpv4|tcpv6] " << std::endl;
        std::cout << "                                       (udpv4 by default)." << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Publisher options:" << std::endl;
        std::cout << "  -t <str>, --topic <str>              Topic name" << std::endl;
        std::cout << "                                       (Default: discovery_server_topic)." << std::endl;
        std::cout << "  -r, --reliable                       Set Reliability QoS as reliable" << std::endl;
        std::cout << "                                       (Default: best effort)" << std::endl;
        std::cout << "      --transient-local                Set Durability QoS as transient local" << std::endl;
        std::cout << "                                       (Default: volatile)" << std::endl;
        std::cout << "  -s <num>, --samples <num>            Number of samples to send " << std::endl;
        std::cout << "                                       (Default: 0 => infinite samples)." << std::endl;
        std::cout << "  -i <num>, --interval <num>           Time between samples in milliseconds" << std::endl;
        std::cout << "                                       (Default: 100)." << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Subscriber options:" << std::endl;
        std::cout << "  -t <str>, --topic <str>              Topic name" << std::endl;
        std::cout << "                                       (Default: discovery_server_topic)." << std::endl;
        std::cout << "  -s <num>, --samples <num>            Number of samples to receive" << std::endl;
        std::cout << "                                       (Default: 0 => infinite samples)." << std::endl;
        std::cout << "  -r, --reliable                       Set Reliability QoS as reliable" << std::endl;
        std::cout << "                                       (Default: best effort)" << std::endl;
        std::cout << "      --transient-local                Set Durability QoS as transient local" << std::endl;
        std::cout << "                                       (Default: volatile)" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Server options:" << std::endl;
        std::cout << "      --listening-address <str>       Server listening address" << std::endl;
        std::cout << "                                       (Default address: 127.0.0.1)" << std::endl;
        std::cout << "      --listening-port <num>          Server listening port" << std::endl;
        std::cout << "                                       (Default port: 16166)" << std::endl;
        std::cout << "      --timeout <num>                 Number of seconds when pdp server to be terminated after no requesting"
                  << std::endl;
        std::cout << "                                       the process (Default: 0 = till ^C)." << std::endl;

        std::cout << "----------------------------------------Example----------------------------------" << std::endl;
        std::cout << R"(.\\dynamic_pdp_discovery.exe pdp_server --listening-address 192.168.31.111 --listening-port 12306--timeout 120)"
                  << std::endl;
        std::cout
            << R"(.\dynamic_pdp_discovery.exe publisher -c 192.168.31.111 -p 12306 -t "test_topic" -s 100 --reliable --transient-local -i 500)"
            << std::endl;
        std::cout
            << R"(.\dynamic_pdp_discovery.exe subscriber -c 192.168.31.111 -p 12306 -t "test_topic" -s 100 --reliable --transient-local)"
            << std::endl;
        std::exit(return_code);
    }

    static bool parse_transport_connection(ds_example_config& config, char* argv[], int& index, bool& uses_ipv6) {
        int len = 0;
        while (argv[len] != nullptr) len++;
        bool was_connection_address_set = false;
        // transport_kind
        if (std::string key = argv[index]; key == "--transport") {
            if (++index < len) {
                if (std::string input = argv[index]; input == "udpv4") {
                    config.pub_config.transport_kind = TransportKind::UDPv4;
                    config.sub_config.transport_kind = TransportKind::UDPv4;
                    config.srv_config.transport_kind = TransportKind::UDPv4;
                } else if (input == "udpv6") {
                    config.pub_config.transport_kind = TransportKind::UDPv6;
                    config.sub_config.transport_kind = TransportKind::UDPv6;
                    config.srv_config.transport_kind = TransportKind::UDPv6;
                    uses_ipv6 = true;
                } else if (input == "tcpv4") {
                    config.pub_config.transport_kind = TransportKind::TCPv4;
                    config.sub_config.transport_kind = TransportKind::TCPv4;
                    config.srv_config.transport_kind = TransportKind::TCPv4;
                } else if (input == "tcpv6") {
                    config.pub_config.transport_kind = TransportKind::TCPv6;
                    config.sub_config.transport_kind = TransportKind::TCPv6;
                    config.srv_config.transport_kind = TransportKind::TCPv6;
                    uses_ipv6 = true;
                } else {
                    EPROSIMA_LOG_ERROR(CLI_PARSER, "Unkown transport argument: " + input);
                    print_help(EXIT_FAILURE);
                }
            } else {
                EPROSIMA_LOG_ERROR(CLI_PARSER, "parsing transport argument");
                print_help(EXIT_FAILURE);
            }
            // Client options
        } else if (key == "-c" || key == "--connection-address") {
            if (++index < len) {
                config.pub_config.connection_address = argv[index];
                config.sub_config.connection_address = argv[index];
                config.srv_config.connection_address = argv[index];
                // if (config.entity == EntityKind::SD_SERVER) {
                //     config.srv_config.is_also_client = true;
                // }
                was_connection_address_set = true;
            } else {
                EPROSIMA_LOG_ERROR(CLI_PARSER, "parsing connection-address argument");
                print_help(EXIT_FAILURE);
            }
        } else if (key == "-p" || key == "--connection-port") {
            if (++index < len) {
                try {
                    if (int input = std::stoi(argv[index]);
                        input < std::numeric_limits<uint16_t>::min() || input > std::numeric_limits<uint16_t>::max()) {
                        throw std::out_of_range("port argument " + std::string(argv[index]) + " out of range [0, 65535].");
                    } else {
                        config.pub_config.connection_port = static_cast<uint16_t>(input);
                        config.sub_config.connection_port = static_cast<uint16_t>(input);
                        config.srv_config.connection_port = static_cast<uint16_t>(input);
                        if (config.entity == EntityKind::SD_SERVER) {
                            config.srv_config.is_also_client = true;
                        }
                    }
                } catch (const std::invalid_argument& e) {
                    EPROSIMA_LOG_ERROR(CLI_PARSER, "invalid port argument " + std::string(argv[index]) + ": " + std::string(e.what()));
                    print_help(EXIT_FAILURE);
                } catch (const std::out_of_range& e) {
                    EPROSIMA_LOG_ERROR(CLI_PARSER, std::string(e.what()));
                    print_help(EXIT_FAILURE);
                }
            } else {
                EPROSIMA_LOG_ERROR(CLI_PARSER, "parsing port argument");
                print_help(EXIT_FAILURE);
            }
        }
        return was_connection_address_set;
    }


    static void parse_pub_sub_config(ds_example_config& config, char* argv[], int& index) {
        int len = 0;
        while (argv[len] != nullptr) len++;
        if (std::string key = argv[index]; key == "-t" || key == "--topic") {
            if (++index < len) {
                if (config.entity == EntityKind::PUBLISHER || config.entity == EntityKind::SUBSCRIBER) {
                    std::cout << "topic name: " << argv[index];
                    config.pub_config.topic_name = argv[index];
                    config.sub_config.topic_name = argv[index];
                } else {
                    EPROSIMA_LOG_ERROR(CLI_PARSER,
                                       "wrong or missing entity for --topic argument: only available for publisher "
                                       "and subscriber");
                    print_help(EXIT_FAILURE);
                }
            } else {
                EPROSIMA_LOG_ERROR(CLI_PARSER, "missing argument for " + key);
                print_help(EXIT_FAILURE);
            }
        } else if (key == "-s" || key == "--samples") {
            if (++index < len) {
                try {
                    if (int input = std::stoi(argv[index]);
                        input < std::numeric_limits<std::uint16_t>::min() || input > std::numeric_limits<std::uint16_t>::max()) {
                        throw std::out_of_range("samples argument out of range");
                    } else {
                        if (config.entity == EntityKind::PUBLISHER || config.entity == EntityKind::SUBSCRIBER) {
                            config.pub_config.samples = static_cast<uint16_t>(input);
                            config.sub_config.samples = static_cast<uint16_t>(input);
                        } else {
                            EPROSIMA_LOG_ERROR(CLI_PARSER, "entity error or not specified for --samples argument");
                            print_help(EXIT_FAILURE);
                        }
                    }
                } catch (const std::invalid_argument& e) {
                    EPROSIMA_LOG_ERROR(CLI_PARSER, "invalid sample argument for " + key + ": " + e.what());
                    print_help(EXIT_FAILURE);
                } catch (const std::out_of_range& e) {
                    EPROSIMA_LOG_ERROR(CLI_PARSER, "samples argument out of range for " + key + ": " + e.what());
                    print_help(EXIT_FAILURE);
                }
            } else {
                EPROSIMA_LOG_ERROR(CLI_PARSER, "missing argument for " + key);
                print_help(EXIT_FAILURE);
            }
        } else if (key == "--reliable") {
            config.pub_config.reliable = true;
            config.sub_config.reliable = true;
        } else if (key == "--transient-local") {
            config.pub_config.transient_local = true;
            config.sub_config.transient_local = true;
        } else if (key == "-i" || key == "--interval") {
            if (config.entity == EntityKind::PUBLISHER) {
                if (++index < len) {
                    try {
                        if (int input = std::stoi(argv[index]);
                            input < std::numeric_limits<std::uint16_t>::min() || input > std::numeric_limits<std::uint16_t>::max()) {
                            throw std::out_of_range("interval argument out of range");
                        } else {
                            config.pub_config.interval = static_cast<uint16_t>(input);
                        }
                    } catch (const std::invalid_argument& e) {
                        EPROSIMA_LOG_ERROR(CLI_PARSER,
                                           "invalid interval argument " + std::string(argv[index]) + ": " + std::string(e.what()));
                        print_help(EXIT_FAILURE);
                    } catch (const std::out_of_range& e) {
                        EPROSIMA_LOG_ERROR(CLI_PARSER, std::string(e.what()));
                        print_help(EXIT_FAILURE);
                    }
                } else {
                    EPROSIMA_LOG_ERROR(CLI_PARSER, "parsing interval argument error");
                    print_help(EXIT_FAILURE);
                }
            } else {
                EPROSIMA_LOG_ERROR(CLI_PARSER, "interval argument is only valid for client publisher entity");
                print_help(EXIT_FAILURE);
            }
        }
    }

    static bool parse_server_config(ds_example_config& config, char* argv[], int& index) {
        int len = 0;
        while (argv[len] != nullptr) len++;
        bool was_listening_address_set = false;
        if (std::string key = argv[index]; key == "--listening-address") {
            if (index + 1 < len) {
                if (config.entity == EntityKind::SD_SERVER) {
                    config.srv_config.listening_address = argv[++index];
                    was_listening_address_set = true;
                } else {
                    EPROSIMA_LOG_ERROR(CLI_PARSER, "listening address  argument is only valid for server entity");
                    print_help(EXIT_FAILURE);
                }
            } else {
                EPROSIMA_LOG_ERROR(CLI_PARSER, "parsing connection-address argument erro");
                print_help(EXIT_FAILURE);
            }
        } else if (key == "--listening-port") {
            if (index + 1 < len) {
                if (config.entity == EntityKind::SD_SERVER) {
                    try {
                        if (int input = std::stoi(argv[++index]);
                            input < std::numeric_limits<uint16_t>::min() || input > std::numeric_limits<uint16_t>::max()) {
                            throw std::out_of_range("listening-port argument " + std::string(argv[++index]) +
                                                    " out of range [0, 65535].");
                        } else {
                            config.srv_config.listening_port = static_cast<uint16_t>(input);
                        }
                    } catch (const std::invalid_argument& e) {
                        EPROSIMA_LOG_ERROR(CLI_PARSER,
                                           "invalid listening-port argument " + std::string(argv[++index]) + ": " +
                                               std::string(e.what()));
                        print_help(EXIT_FAILURE);
                    } catch (const std::out_of_range& e) {
                        EPROSIMA_LOG_ERROR(CLI_PARSER, std::string(e.what()));
                        print_help(EXIT_FAILURE);
                    }
                } else {
                    EPROSIMA_LOG_ERROR(CLI_PARSER, "--listening-port argument is only valid for server entity");
                    print_help(EXIT_FAILURE);
                }
            } else {
                EPROSIMA_LOG_ERROR(CLI_PARSER, "parsing port argument");
                print_help(EXIT_FAILURE);
            }
        } else if (key == "--timeout") {
            if (index + 1 < len) {
                if (config.entity == EntityKind::SD_SERVER) {
                    try {
                        if (int input = std::stoi(argv[++index]);
                            input < std::numeric_limits<uint16_t>::min() || input > std::numeric_limits<uint16_t>::max()) {
                            throw std::out_of_range("timeout argument " + std::string(argv[++index]) + " out of range [0, 65535].");
                        } else {
                            config.srv_config.timeout = static_cast<uint16_t>(input);
                        }
                    } catch (const std::invalid_argument& e) {
                        EPROSIMA_LOG_ERROR(CLI_PARSER,
                                           "invalid timeout argument " + std::string(argv[++index]) + ": " + std::string(e.what()));
                        print_help(EXIT_FAILURE);
                    } catch (const std::out_of_range& e) {
                        EPROSIMA_LOG_ERROR(CLI_PARSER, std::string(e.what()));
                        print_help(EXIT_FAILURE);
                    }
                } else {
                    EPROSIMA_LOG_ERROR(CLI_PARSER, "--listening-port argument is only valid for server entity");
                    print_help(EXIT_FAILURE);
                }
            } else {
                EPROSIMA_LOG_ERROR(CLI_PARSER, "parsing port argument");
                print_help(EXIT_FAILURE);
            }
        } else {
            EPROSIMA_LOG_ERROR(CLI_PARSER, "unknown option " + key);
            print_help(EXIT_FAILURE);
        }
        return was_listening_address_set;
    }
    /**
     * @brief Parse the command line options and return the configuration_config object
     *
     * @param argc number of arguments
     * @param argv array of arguments
     * @return configuration_config object with the parsed options
     *
     * @warning This method finishes the execution of the program if the input arguments are invalid
     */
    static ds_example_config parse_cli_options(int argc, char* argv[]) {
        ds_example_config config;

        if (argc < 2) {
            EPROSIMA_LOG_ERROR(CLI_PARSER, "missing entity argument");
            print_help(EXIT_FAILURE);
        }

        if (std::string first_argument = argv[1]; first_argument == "publisher") {
            config.entity = EntityKind::PUBLISHER;
        } else if (first_argument == "subscriber") {
            config.entity = EntityKind::SUBSCRIBER;
        } else if (first_argument == "pdp_server") {
            config.entity = EntityKind::SD_SERVER;
        } else if (first_argument == "-h" || first_argument == "--help") {
            print_help(EXIT_SUCCESS);
        } else {
            EPROSIMA_LOG_ERROR(CLI_PARSER, "parsing entity argument " + first_argument);
            print_help(EXIT_FAILURE);
        }

        bool uses_ipv6 = false;
        bool listening_address_was_set = false;
        bool connection_address_was_set = false;

        for (int i = 2; i < argc; ++i) {
            if (std::string arg = argv[i]; arg == "-h" || arg == "--help") {
                print_help(EXIT_SUCCESS);
                // Common options
            } else if (arg == "--transport" || arg == "-c" || arg == "--connection-address" || arg == "-p" ||
                       arg == "--connection-port") {
                connection_address_was_set = parse_transport_connection(config, argv, i, uses_ipv6);
                // PubSub options
            } else if (arg == "-t" || arg == "--topic" || arg == "-s" || arg == "--samples" || arg == "--reliable" ||
                       arg == "--transient-local" || arg == "-i" || arg == "--interval") {
                parse_pub_sub_config(config, argv, i);

                // Server options
            } else if (arg == "--listening-address" || arg == "--listening-port" || arg == "--timeout") {
                listening_address_was_set = parse_server_config(config, argv, i);
            }
        }
        // change default values if IPv6 is used
        // and user did not specified ones
        if (uses_ipv6) {
            if (config.entity == EntityKind::SD_SERVER && !listening_address_was_set) {
                config.srv_config.listening_address = "::1";
            }

            if (!connection_address_was_set) {
                config.pub_config.connection_address = "::1";
                config.sub_config.connection_address = "::1";
                if (config.srv_config.is_also_client) {
                    config.srv_config.connection_address = "::1";
                }
            }
        }
        std::cout << config << std::endl;
        return config;
    }

    /**
     * @brief Parse the signal number into the signal name
     *
     * @param signum signal number
     * @return std::string signal name
     */
    static std::string parse_signal(const int& signum) {
        switch (signum) {
        case SIGINT:
            return "SIGINT";
        case SIGTERM:
            return "SIGTERM";
#ifndef _WIN32
        case SIGQUIT:
            return "SIGQUIT";
        case SIGHUP:
            return "SIGHUP";
#endif // _WIN32
        default:
            return "UNKNOWN SIGNAL";
        }
    }

    /**
     * @brief Parse the entity kind into std::string
     *
     * @param entity entity kind
     * @return std::string entity kind
     */
    static std::string parse_entity_kind(const EntityKind& entity) {
        switch (entity) {
        case EntityKind::PUBLISHER:
            return "Publisher";
        case EntityKind::SUBSCRIBER:
            return "Subscriber";
        case EntityKind::SD_SERVER:
            return "Service Discovery Server";
        case EntityKind::UNDEFINED:
        default:
            return "Undefined entity";
        }
    }
};

} // namespace discovery_server
} // namespace examples
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_EXAMPLES_CPP_DISCOVERY_SERVER__CLIPARSER_HPP
