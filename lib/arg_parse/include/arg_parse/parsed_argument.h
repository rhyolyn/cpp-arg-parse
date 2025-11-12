#include <arg_parse/argument.h>
#include <memory>
#include <vector>
#include <string>

class ParsedArgument : public Argument {
public:
    explicit ParsedArgument(const std::string& name)
        : Argument(name), options(nullptr) {}

    // Constructor with options
    ParsedArgument(const std::string& name, std::vector<std::string> option_values)
        : Argument(name), options(std::make_unique<std::vector<std::string>>(std::move(option_values))) {}

    // Default move constructor and move assignment (needed for unique_ptr)
    ParsedArgument(ParsedArgument&&) = default;
    ParsedArgument& operator=(ParsedArgument&&) = default;

    // Delete copy constructor and copy assignment (unique_ptr is not copyable)
    ParsedArgument(const ParsedArgument&) = delete;
    ParsedArgument& operator=(const ParsedArgument&) = delete;

    // Check if options are present
    bool has_options() const {
        return options != nullptr;
    }

    // Get options (returns empty vector if no options)
    std::vector<std::string> get_options() const {
        return options ? *options : std::vector<std::string>{};
    }

    // Get number of options
    unsigned get_num_options() const {
        return options ? static_cast<unsigned>(options->size()) : 0;
    }

private:
    std::unique_ptr<std::vector<std::string>> options;  // Optional: nullptr if no options
};