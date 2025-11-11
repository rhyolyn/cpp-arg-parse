#pragma once
#include <iostream>
#include <vector>
#include <string>

#include <common/api.h>
#include <arg_parse/argument.h>

// Result structure for parsed arguments (moved from minimal_parser.h)
struct API ParseResult {
    bool success;
    std::string ip;
    std::string port;
    std::string error_message;
};

class API ArgumentParser
{
public:
    ArgumentParser();
    ~ArgumentParser();
    ArgumentParser(const ArgumentParser&);
    ArgumentParser(ArgumentParser&&);

    void add_argument(const Argument& arg);

    // Previously free function parse(...) in minimal_parser.cpp.
    // Now a member of ArgumentParser.
    ParseResult parse(int argc, char** argv);

private:
    std::vector<Argument> arguments;
};