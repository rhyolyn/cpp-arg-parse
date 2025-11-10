#include <algorithm>
#include <arg_parse/argument.h>


void Argument::set_name(const std::string& decorated_name) {
    name = decorated_name;
    remove_leading_dashes(name);
    replace_all_dashes_with_underscores(name);
}

void Argument::remove_leading_dashes(std::string& str) {
    if (str.size() >= DASH_DASH_LENGTH &&
        str.substr(0, DASH_DASH_LENGTH) == DASH_DASH) {
        str = str.substr(DASH_DASH_LENGTH);
    }
}

void Argument::replace_all_dashes_with_underscores(std::string& str) {
    std::replace(str.begin(), str.end(), DASH, UNDERSCORE);
}