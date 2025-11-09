#pragma once

#pragma once

#include <iostream>
#include <vector>

#include <common/api.h>
#include <arg_parse/argument.h>

class API ArgumentParser
{
public:
    ArgumentParser();
    ~ArgumentParser();
    ArgumentParser(const ArgumentParser&);
    ArgumentParser(ArgumentParser&&);

    void add_argument(const Argument& arg);

private:
    std::vector<Argument> arguments;
};
