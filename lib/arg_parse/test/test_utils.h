#pragma once

#include <gtest/gtest.h>
#include <algorithm>
#include <string>

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