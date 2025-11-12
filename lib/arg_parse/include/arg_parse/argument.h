#pragma once
#include <string>
#include <string_view>
#include <algorithm>
#include <cctype>
#include <stdexcept>

class Argument {
public:
    static constexpr std::string_view ILLEGAL_CHARS = " -.,!@#$%&*()[]{}+=|\\:;\"'<>?/~`";
    static constexpr char REPLACEMENT_CHAR = '_';

    explicit Argument(const std::string& name) : name(name) {
        canonicalize_name();
    }

    std::string get_name() const {
        return name;
    }

protected:
    void canonicalize_name() {
        std::string original_name = name;  // Save for error message
        replace_illegal_characters();
        strip_leading_non_alphas();
        throw_if_empty(original_name);
    }

    void replace_illegal_characters() {
        for (char illegal : ILLEGAL_CHARS) {
            std::replace(name.begin(), name.end(), illegal, REPLACEMENT_CHAR);
        }
    }

    void strip_leading_non_alphas() {
        auto it = std::find_if(name.begin(), name.end(), [](unsigned char c) {
            return std::isalpha(c);
        });
        name.erase(name.begin(), it);
    }

    void throw_if_empty(const std::string& original_name) const {
        if (name.empty()) {
            throw std::invalid_argument(
                "Argument name cannot be empty after canonicalization. Original name: '" +
                original_name + "'"
            );
        }
    }

private:
    std::string name;
};
