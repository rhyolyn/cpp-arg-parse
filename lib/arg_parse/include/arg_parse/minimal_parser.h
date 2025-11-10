#pragma once

#include <string>
#include <common/api.h>

// Result structure for parsed arguments
struct API ParseResult {
    bool success;
    std::string ip;
    std::string port;
    std::string error_message;
};

// Minimal command line parser that takes two arguments with one 
// option each: --ip <ip_address> and --port <port_number>
API ParseResult parse(int argc, char** argv);