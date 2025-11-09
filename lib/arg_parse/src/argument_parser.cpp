#include <iostream>

#include <common/api.h>
#include <arg_parse/argument.h>
#include <arg_parse/argument_parser.h>


ArgumentParser::ArgumentParser() {}

ArgumentParser::~ArgumentParser() {}

ArgumentParser::ArgumentParser(const ArgumentParser&) {}

ArgumentParser::ArgumentParser(ArgumentParser&&) {}

void ArgumentParser::add_argument(const Argument& arg)
{
    arguments.push_back(arg);
}

