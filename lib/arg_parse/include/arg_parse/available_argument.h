#include <arg_parse/argument.h>

class AvailableArgument : public Argument {
public:
    explicit AvailableArgument(const std::string& name, std::string description, unsigned num_options = 0) : 
        Argument(name), description(std::move(description)), num_options(num_options) {}
    std::string get_description() const { return description; }
    unsigned get_num_options() const { return num_options; }

private:    
    std::string description;
    unsigned num_options;
};