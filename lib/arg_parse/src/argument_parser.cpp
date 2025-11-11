#include <iostream>
#include <sstream>

#include <common/api.h>
#include <arg_parse/argument.h>
#include <arg_parse/argument_parser.h>

ArgumentParser::ArgumentParser() {}
ArgumentParser::~ArgumentParser() {}
ArgumentParser::ArgumentParser(const ArgumentParser&) = default;
ArgumentParser::ArgumentParser(ArgumentParser&&) = default;

void ArgumentParser::add_argument(const Argument& arg)
{
    arguments.push_back(arg);
}

// Minimal command line parsing logic that used to live in minimal_parser.cpp.
// Supports: --ip <ip_address> --port <port_number>
ParseResult ArgumentParser::parse(int argc, char** argv)
{
    // If the parser has no arguments defined, populate the two minimal ones.
    if (arguments.empty()) {
        add_argument(Argument("--ip", "IP address to connect to", 1, true));
        add_argument(Argument("--port", "Port number to connect to", 1, true));
    }

    std::string ip;
    std::string port;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ip") {
            if (i + 1 < argc) {
                ip = argv[++i];
            }
            else {
                return ParseResult{ false, "", "", "Error: --ip requires a value" };
            }
        }
        else if (arg == "--port") {
            if (i + 1 < argc) {
                port = argv[++i];
            }
            else {
                return ParseResult{ false, "", "", "Error: --port requires a value" };
            }
        }
        else {
            std::ostringstream oss;
            oss << "Unknown argument: " << arg;
            return ParseResult{ false, "", "", oss.str() };
        }
    }

    if (ip.empty() || port.empty()) {
        std::ostringstream oss;
        oss << "Usage: " << (argc > 0 ? argv[0] : "program") << " --ip <ip_address> --port <port_number>";
        return ParseResult{ false, "", "", oss.str() };
    }

    return ParseResult{ true, ip, port, "" };
}