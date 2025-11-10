#include <iostream>
#include <string>
#include <sstream>
#include <arg_parse/minimal_parser.h>
#include <arg_parse/argument.h>
#include <arg_parse/argument_parser.h>


// minimal command line parser that takes two arguments with one 
// option each: --ip <ip_address> and --port <port_number>
ParseResult parse(int argc, char** argv) {
    ArgumentParser parser;
    parser.add_argument(Argument("--ip", "IP address to connect to", 1, true));
    parser.add_argument(Argument("--port", "Port number to connect to", 1, true));

    // Simple command line parsing logic
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
        oss << "Usage: " << argv[0] << " --ip <ip_address> --port <port_number>";
        return ParseResult{ false, "", "", oss.str() };
    }

    return ParseResult{ true, ip, port, "" };
}