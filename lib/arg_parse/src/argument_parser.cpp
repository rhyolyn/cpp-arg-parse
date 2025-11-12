#include <arg_parse/argument_parser.h>

ArgumentParser::ArgumentParser() = default;
ArgumentParser::~ArgumentParser() = default;

void ArgumentParser::add_argument(const AvailableArgument& arg) {
    available_arguments.push_back(arg);
}

void ArgumentParser::parse(int argc, char** argv) {
    parsed_arguments.clear();
    for (int i = 1; i < argc; ++i) {
        parsed_arguments.push_back(ParsedArgument(argv[i]));
    }
}

const std::vector<AvailableArgument>& ArgumentParser::get_available_arguments() const {
    return available_arguments;
}

const std::vector<ParsedArgument>& ArgumentParser::get_parsed_arguments() const {
    return parsed_arguments;
}

