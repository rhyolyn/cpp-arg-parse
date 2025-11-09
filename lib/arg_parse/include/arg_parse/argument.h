#pragma once

#include <string>

class Argument
{
public:
    Argument(const std::string& name,
        const std::string& description,
        unsigned num_options,
        bool required)
        : name(name), description(description), num_options(num_options), required(required)
    {
    }

    // Getters
    const std::string& get_name() const { return name; }
    const std::string& get_description() const { return description; }
    unsigned get_num_options() const { return num_options; }
    bool is_required() const { return required; }

private:
    std::string name;
    std::string description;
    unsigned num_options;
    bool required;
};