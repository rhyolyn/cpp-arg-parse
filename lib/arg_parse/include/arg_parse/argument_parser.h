#pragma once
#include <vector>
#include <string>
#include <common/api.h>
#include <arg_parse/argument.h>
#include <arg_parse/available_argument.h>
#include <arg_parse/parsed_argument.h>

class API ArgumentParser
{
public:
    ArgumentParser();
    ~ArgumentParser();

    // Delete copy operations (because ParsedArgument is not copyable)
    ArgumentParser(const ArgumentParser&) = delete;
    ArgumentParser& operator=(const ArgumentParser&) = delete;

    // Default move operations (ParsedArgument is movable)
    ArgumentParser(ArgumentParser&&) = default;
    ArgumentParser& operator=(ArgumentParser&&) = default;

    void add_argument(const AvailableArgument& arg);
    void parse(int argc, char** argv);

    const std::vector<AvailableArgument>& get_available_arguments() const;
    const std::vector<ParsedArgument>& get_parsed_arguments() const;

private:
    std::vector<AvailableArgument> available_arguments;
    std::vector<ParsedArgument> parsed_arguments;

};