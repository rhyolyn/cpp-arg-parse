#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <arg_parse/minimal_parser.h>
#include "test_utils.h"

// Helper struct for minimal parser test data
struct MinimalParserTestCase {
    std::vector<std::string> args;
    bool expected_success;
    std::string expected_ip;
    std::string expected_port;
};

// Convert vector<string> to argc/argv format for testing
class ArgvHelper {
public:
    ArgvHelper(const std::vector<std::string>& args) {
        for (const auto& arg : args) {
            char* cstr = new char[arg.size() + 1];
            std::strcpy(cstr, arg.c_str());
            argv_.push_back(cstr);
        }
    }

    ~ArgvHelper() {
        for (char* arg : argv_) {
            delete[] arg;
        }
    }

    int argc() const { return static_cast<int>(argv_.size()); }
    char** argv() { return argv_.data(); }

private:
    std::vector<char*> argv_;
};

class MinimalParserTest : public ::testing::Test {};

// Parameterized test fixture
class MinimalParserParameterizedTest : public ::testing::TestWithParam<TestCase<MinimalParserTestCase>> {};

TEST_P(MinimalParserParameterizedTest, ParseBehavior) {
    auto test_case = GetParam();
    auto& test_data = test_case.value;

    ArgvHelper helper(test_data.args);
    ParseResult result = parse(helper.argc(), helper.argv());

    EXPECT_EQ(result.success, test_data.expected_success)
        << "Test: " << test_case.description;

    if (test_data.expected_success) {
        EXPECT_EQ(result.ip, test_data.expected_ip)
            << "Test: " << test_case.description;
        EXPECT_EQ(result.port, test_data.expected_port)
            << "Test: " << test_case.description;
    }
}

INSTANTIATE_TEST_SUITE_P(
    ParserCases,
    MinimalParserParameterizedTest,
    ::testing::Values(
        // Success cases
        TestCase<MinimalParserTestCase>{{{"program", "--ip", "127.0.0.1", "--port", "8080"}, true, "127.0.0.1", "8080"}, "ParsesIpAndPort"},
        TestCase<MinimalParserTestCase>{{{"program", "--port", "8080", "--ip", "127.0.0.1"}, true, "127.0.0.1", "8080"}, "ParsesIpAndPortInReverseOrder"},

        // Missing arguments
        TestCase<MinimalParserTestCase>{{{"program", "--port", "8080"}, false, "", ""}, "FailsWithMissingIp"},
        TestCase<MinimalParserTestCase>{{{"program", "--ip", "127.0.0.1"}, false, "", ""}, "FailsWithMissingPort"},
        TestCase<MinimalParserTestCase>{{{"program"}, false, "", ""}, "FailsWithNoArguments"},

        // Missing values
        TestCase<MinimalParserTestCase>{{{"program", "--ip"}, false, "", ""}, "FailsWhenIpRequiresValue"},
        TestCase<MinimalParserTestCase>{{{"program", "--port"}, false, "", ""}, "FailsWhenPortRequiresValue"},
        TestCase<MinimalParserTestCase>{{{"program", "--port", "8080", "--ip"}, false, "", ""}, "FailsWhenIpRequiresValueAtEnd"},
        TestCase<MinimalParserTestCase>{{{"program", "--ip", "127.0.0.1", "--port"}, false, "", ""}, "FailsWhenPortRequiresValueAtEnd"},
        TestCase<MinimalParserTestCase>{{{"program", "--ip", "--port"}, false, "", ""}, "FailsWhenIpValueIsPortFlag"},
        TestCase<MinimalParserTestCase>{{{"program", "--port", "--ip"}, false, "", ""}, "FailsWhenPortValueIsIpFlag"},

        // Unknown/invalid arguments
        TestCase<MinimalParserTestCase>{{{"program", "--unknown", "value", "--ip", "127.0.0.1", "--port", "8080"}, false, "", ""}, "FailsWithUnknownArgument"},
        TestCase<MinimalParserTestCase>{{{"program", "--ip", "127.0.0.1", "--port", "8080", "extra"}, false, "", ""}, "FailsWithExtraArgument"},
        TestCase<MinimalParserTestCase>{{{"program", "positional", "--ip", "127.0.0.1", "--port", "8080"}, false, "", ""}, "FailsWithPositionalArgument"}
    ),
    TestCaseNamer<MinimalParserTestCase>()
);