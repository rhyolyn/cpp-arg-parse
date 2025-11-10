#include <gtest/gtest.h>

#include "arg_parse/argument.h"
#include "arg_parse/argument_parser.h"
#include "test_utils.h"


TEST(ArgParserTest, CanInstantiate) {
    ArgumentParser parser;
    SUCCEED();
}

// Parameterized test fixture using TestCase wrapper
class ArgParserAddArgumentTest : public ::testing::TestWithParam<TestCase<Argument>> {};

TEST_P(ArgParserAddArgumentTest, CanAddArgument) {
    ArgumentParser parser;
    auto test_case = GetParam();
    Argument arg = test_case.value;

    // Should not throw
    EXPECT_NO_THROW(parser.add_argument(arg))
        << "Test: " << test_case.description
        << "\nFailed to add argument: '" << arg.get_name() << "'"
        << "\n  Description: " << arg.get_description()
        << "\n  Num Options: " << arg.get_num_options()
        << "\n  Required: " << (arg.is_required() ? "true" : "false");
}

// Instantiate with descriptive test names
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