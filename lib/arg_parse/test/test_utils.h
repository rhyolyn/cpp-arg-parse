#pragma once

#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cstring>

// Generic test case wrapper
template<typename T>
struct TestCase {
    T value;
    std::string description;
};

// Generic naming function for TestCase
template<typename T>
struct TestCaseNamer {
    std::string operator()(const ::testing::TestParamInfo<TestCase<T>>& info) const {
        std::string name = info.param.description;
        std::replace(name.begin(), name.end(), ' ', '_');
        std::replace(name.begin(), name.end(), '-', '_');
        std::replace(name.begin(), name.end(), '/', '_');
        std::replace(name.begin(), name.end(), '.', '_');
        return name.empty() ? "unnamed" : name;
    }
};


class MockArgv {
public:
    explicit MockArgv(const std::vector<std::string>& arguments)
        : argc(static_cast<int>(arguments.size())) {
        argv.reserve(arguments.size());
        for (const auto& arg : arguments) {
            char* buffer = new char[arg.size() + 1];
            std::strcpy(buffer, arg.c_str());
            argv.push_back(buffer);
        }
    }

    ~MockArgv() {
        for (char* arg : argv) {
            delete[] arg;
        }
    }

    MockArgv(const MockArgv&) = delete;
    MockArgv& operator=(const MockArgv&) = delete;
    MockArgv(MockArgv&&) = delete;
    MockArgv& operator=(MockArgv&&) = delete;

    int getArgc() const { return argc; }
    char** getArgv() { return argv.data(); }

private:
    int argc;
    std::vector<char*> argv;
};