#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <cstring>

#include <arg_parse/argument.h>
#include <arg_parse/argument_parser.h>
#include "test_utils.h"

TEST(ArgParserTest, CanInstantiate) {
    ArgumentParser parser;
    SUCCEED();
}

// Parameterized test fixture using TestCase wrapper for add_argument
class ArgParserAddArgumentTest : public ::testing::TestWithParam<TestCase<Argument>> {};

TEST_P(ArgParserAddArgumentTest, CanAddArgument) {
    ArgumentParser parser;
    auto test_case = GetParam();
    Argument arg = test_case.value;

    EXPECT_NO_THROW(parser.add_argument(arg))
        << "Test: " << test_case.description
        << "\nFailed to add argument: '" << arg.get_name() << "'"
        << "\n  Description: " << arg.get_description()
        << "\n  Num Options: " << arg.get_num_options()
        << "\n  Required: " << (arg.is_required() ? "true" : "false");
}

INSTANTIATE_TEST_SUITE_P(
    MultipleArguments,
    ArgParserAddArgumentTest,
    ::testing::Values(
        TestCase<Argument>{ Argument{ "--input", "Input file", 0, false }, "FlagWithNoOptions" },
        TestCase<Argument>{ Argument{ "--output", "Output file", 1, true }, "RequiredSingleOption" },
        TestCase<Argument>{ Argument{ "--config", "Config files", 2, false }, "OptionalTwoOptions" },
        TestCase<Argument>{ Argument{ "--data", "Data files", 3, true }, "RequiredThreeOptions" },
        TestCase<Argument>{ Argument{ "--verbose", "Verbose mode", 0, false }, "VerboseFlag" }
    ),
    TestCaseNamer<Argument>()
);

// Test cases for ArgumentParser::parse (formerly minimal parser logic)
struct ArgumentParserParseTestCase {
    std::vector<std::string> args;
    bool expected_success;
    std::string expected_ip;
    std::string expected_port;
};

// Helper to build argc/argv
class ArgvHelper {
public:
    explicit ArgvHelper(const std::vector<std::string>& args) {
        for (const auto& a : args) {
            char* cstr = new char[a.size() + 1];
            std::strcpy(cstr, a.c_str());
            argv_.push_back(cstr);
        }
    }
    ~ArgvHelper() {
        for (char* p : argv_) delete[] p;
    }
    int argc() const { return static_cast<int>(argv_.size()); }
    char** argv() { return argv_.data(); }
private:
    std::vector<char*> argv_;
};

// Parameterized fixture for parse behavior
class ArgumentParserParseTest : public ::testing::TestWithParam<TestCase<ArgumentParserParseTestCase>> {};

TEST_P(ArgumentParserParseTest, ParseBehavior) {
    auto test_case = GetParam();
    auto& td = test_case.value;

    ArgvHelper helper(td.args);
    ArgumentParser parser;
    ParseResult result = parser.parse(helper.argc(), helper.argv());

    EXPECT_EQ(result.success, td.expected_success)
        << "Test: " << test_case.description;

    if (td.expected_success) {
        EXPECT_EQ(result.ip, td.expected_ip) << "Test: " << test_case.description;
        EXPECT_EQ(result.port, td.expected_port) << "Test: " << test_case.description;
    }
}

INSTANTIATE_TEST_SUITE_P(
    ParseFunctionCases,
    ArgumentParserParseTest,
    ::testing::Values(
        // Success cases
        TestCase<ArgumentParserParseTestCase>{{{"program", "--ip", "127.0.0.1", "--port", "8080"}, true, "127.0.0.1", "8080"}, "ParsesIpAndPort"},
        TestCase<ArgumentParserParseTestCase>{{{"program", "--port", "8080", "--ip", "127.0.0.1"}, true, "127.0.0.1", "8080"}, "ParsesIpAndPortInReverseOrder"},

        // Missing arguments
        TestCase<ArgumentParserParseTestCase>{{{"program", "--port", "8080"}, false, "", ""}, "FailsWithMissingIp"},
        TestCase<ArgumentParserParseTestCase>{{{"program", "--ip", "127.0.0.1"}, false, "", ""}, "FailsWithMissingPort"},
        TestCase<ArgumentParserParseTestCase>{{{"program"}, false, "", ""}, "FailsWithNoArguments"},

        // Missing values
        TestCase<ArgumentParserParseTestCase>{{{"program", "--ip"}, false, "", ""}, "FailsWhenIpRequiresValue"},
        TestCase<ArgumentParserParseTestCase>{{{"program", "--port"}, false, "", ""}, "FailsWhenPortRequiresValue"},
        TestCase<ArgumentParserParseTestCase>{{{"program", "--port", "8080", "--ip"}, false, "", ""}, "FailsWhenIpRequiresValueAtEnd"},
        TestCase<ArgumentParserParseTestCase>{{{"program", "--ip", "127.0.0.1", "--port"}, false, "", ""}, "FailsWhenPortRequiresValueAtEnd"},
        TestCase<ArgumentParserParseTestCase>{{{"program", "--ip", "--port"}, false, "", ""}, "FailsWhenIpValueIsPortFlag"},
        TestCase<ArgumentParserParseTestCase>{{{"program", "--port", "--ip"}, false, "", ""}, "FailsWhenPortValueIsIpFlag"},

        // Unknown / invalid
        TestCase<ArgumentParserParseTestCase>{{{"program", "--unknown", "value", "--ip", "127.0.0.1", "--port", "8080"}, false, "", ""}, "FailsWithUnknownArgument"},
        TestCase<ArgumentParserParseTestCase>{{{"program", "--ip", "127.0.0.1", "--port", "8080", "extra"}, false, "", ""}, "FailsWithExtraArgument"},
        TestCase<ArgumentParserParseTestCase>{{{"program", "positional", "--ip", "127.0.0.1", "--port", "8080"}, false, "", ""}, "FailsWithPositionalArgument"}
    ),
    TestCaseNamer<ArgumentParserParseTestCase>()
);