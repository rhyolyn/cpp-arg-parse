#include <gtest/gtest.h>
#include <arg_parse/argument_parser.h>
#include <arg_parse/argument.h>
#include "test_utils.h"

// Parameterized test fixture using TestCase wrapper
class ArgumentParameterizedTest : public ::testing::TestWithParam<TestCase<Argument>> {};

TEST_P(ArgumentParameterizedTest, CanConstructWithParameters) {
    auto test_case = GetParam();
    auto expected = test_case.value;

    Argument arg{ expected.get_name(), expected.get_description(), expected.get_num_options(), expected.is_required() };

    EXPECT_EQ(arg.get_name(), expected.get_name());
    EXPECT_EQ(arg.get_description(), expected.get_description());
    EXPECT_EQ(arg.get_num_options(), expected.get_num_options());
    EXPECT_EQ(arg.is_required(), expected.is_required());
}

INSTANTIATE_TEST_SUITE_P(
    ArgumentTests,
    ArgumentParameterizedTest,
    ::testing::Values(
        TestCase<Argument>{ Argument{ "--input", "Input file", 1, true }, "InputWithOneOption" },
        TestCase<Argument>{ Argument{ "--output", "Output file", 1, false }, "OutputWithOneOption" },
        TestCase<Argument>{ Argument{ "--verbose", "Verbose mode", 0, false }, "VerboseFlagWithNoOptions" },
        TestCase<Argument>{ Argument{ "--config", "Config files", 3, true }, "ConfigWithThreeOptions" },
        TestCase<Argument>{ Argument{ "", "Empty name", 0, false }, "EmptyName" }
    ),
    TestCaseNamer<Argument>()
);


class ArgumentTest : public ::testing::Test {};

TEST_F(ArgumentTest, CanCopyArgument) {
    Argument arg1{ "--input", "Input file", 1, true };
    Argument arg2 = arg1;

    EXPECT_EQ(arg2.get_name(), arg1.get_name());
    EXPECT_EQ(arg2.get_description(), arg1.get_description());
    EXPECT_EQ(arg2.get_num_options(), arg1.get_num_options());
    EXPECT_EQ(arg2.is_required(), arg1.is_required());
}

TEST_F(ArgumentTest, ConstructorProcessesName) {
    Argument arg{ "--my-option", "Test", 1, false };

    // Verify the constructor processed the name
    EXPECT_EQ(arg.get_name(), "my_option");
}

// Helper struct for set_name test data
struct SetNameTestCase {
    std::string input_name;
    std::string expected_name;
};

// Parameterized test fixture for set_name
class ArgumentSetNameTest : public ::testing::TestWithParam<TestCase<SetNameTestCase>> {};

TEST_P(ArgumentSetNameTest, SetNameConvertsToValidVariableName) {
    auto test_case = GetParam();
    std::string input = test_case.value.input_name;
    std::string expected = test_case.value.expected_name;

    Argument arg{ "initial", "Test argument", 1, false };
    arg.set_name(input);

    EXPECT_EQ(arg.get_name(), expected)
        << "Test: " << test_case.description
        << "\n  Input: '" << input << "'"
        << "\n  Expected: '" << expected << "'"
        << "\n  Actual: '" << arg.get_name() << "'";
}

INSTANTIATE_TEST_SUITE_P(
    VariableNameConversion,
    ArgumentSetNameTest,
    ::testing::Values(
        TestCase<SetNameTestCase>{ {"--input-file", "input_file"}, "RemovesLeadingDashes" },
        TestCase<SetNameTestCase>{ {"--output", "output"}, "RemovesLeadingDashesSimple" },
        TestCase<SetNameTestCase>{ {"--my-long-option", "my_long_option"}, "ReplacesMultipleDashes" },
        TestCase<SetNameTestCase>{ {"no-leading-dashes", "no_leading_dashes"}, "NoLeadingDashesReplaces" },
        TestCase<SetNameTestCase>{ {"--verbose", "verbose"}, "SingleWordWithDashes" },
        TestCase<SetNameTestCase>{ {"simple", "simple"}, "NoChangeNeeded" },
        TestCase<SetNameTestCase>{ {"--", ""}, "OnlyDashes" },
        TestCase<SetNameTestCase>{ {"", ""}, "EmptyString" },
        TestCase<SetNameTestCase>{ {"--a-b-c-d", "a_b_c_d"}, "MultipleSingleCharSegments" },
        TestCase<SetNameTestCase>{ {"---triple", "_triple"}, "TripleLeadingDashes" }
    ),
    TestCaseNamer<SetNameTestCase>()
);