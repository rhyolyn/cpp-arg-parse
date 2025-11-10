#pragma once
#include <string>
#include <algorithm>

#include <common/api.h>

class API Argument
{
public:
    static constexpr const char* DASH_DASH = "--";
    static constexpr size_t DASH_DASH_LENGTH = 2;
    static constexpr char DASH = '-';
    static constexpr char UNDERSCORE = '_';

    Argument(const std::string& name,
        const std::string& description,
        unsigned num_options,
        bool required)
        : description(description), num_options(num_options), required(required)
    {
        this->set_name(name);
    }

    // Getters
    const std::string& get_name() const { return name; }
    const std::string& get_description() const { return description; }
    unsigned get_num_options() const { return num_options; }
    bool is_required() const { return required; }

    // Setters
    void set_name(const std::string& new_name);

    // Helpers
    static void remove_leading_dashes(std::string& str);
    static void replace_all_dashes_with_underscores(std::string& str);

private:
    std::string name;
    std::string description;
    unsigned num_options;
    bool required;
};