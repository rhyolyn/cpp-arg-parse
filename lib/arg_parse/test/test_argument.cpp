#include <gtest/gtest.h>
#include <arg_parse/argument.h>
#include "test_utils.h"

// ============================================================================
// Tests for Argument Constructor (which calls canonicalize_name())
// ============================================================================

struct ArgumentConstructorTestCase {
    std::string input;
    std::string expected;
};

class ArgumentConstructorTest : public ::testing::TestWithParam<TestCase<ArgumentConstructorTestCase>> {};

TEST_P(ArgumentConstructorTest, ConstructorCanonicalizes) {
    auto test_case = GetParam();

    Argument arg(test_case.value.input);

    EXPECT_EQ(arg.get_name(), test_case.value.expected)
        << "Test: " << test_case.description
        << "\nInput: '" << test_case.value.input << "'"
        << "\nExpected: '" << test_case.value.expected << "'"
        << "\nActual: '" << arg.get_name() << "'";
}

INSTANTIATE_TEST_SUITE_P(
    CanonicalizeOnConstruction,
    ArgumentConstructorTest,
    ::testing::Values(
        // Replace illegal characters and strip leading non-alphas
        TestCase<ArgumentConstructorTestCase>{{"--ip-address", "ip_address"}, "ConvertsDoubleLeadingDashWithInternalDash"},
        TestCase<ArgumentConstructorTestCase>{{"-verbose", "verbose"}, "ConvertsSingleLeadingDash"},
        TestCase<ArgumentConstructorTestCase>{{"--my-long-option", "my_long_option"}, "ConvertsMultipleDashes"},
        TestCase<ArgumentConstructorTestCase>{{"--a-b-c-d", "a_b_c_d"}, "ConvertsMultipleSegments"},

        // Leading special characters replaced then stripped
        TestCase<ArgumentConstructorTestCase>{{"!important", "important"}, "StripsLeadingExclamation"},
        TestCase<ArgumentConstructorTestCase>{{"@username", "username"}, "StripsLeadingAt"},
        TestCase<ArgumentConstructorTestCase>{{".hidden", "hidden"}, "StripsLeadingDot"},
        TestCase<ArgumentConstructorTestCase>{{" space", "space"}, "StripsLeadingSpace"},
        TestCase<ArgumentConstructorTestCase>{{"___underscore", "underscore"}, "StripsLeadingUnderscores"},
        TestCase<ArgumentConstructorTestCase>{{"---triple", "triple"}, "StripsLeadingTripleDash"},

        // Mixed illegal characters
        TestCase<ArgumentConstructorTestCase>{{"--my-option.name", "my_option_name"}, "ReplacesDashAndDot"},
        TestCase<ArgumentConstructorTestCase>{{"--option name!", "option_name_"}, "ReplacesSpaceAndExclamation"},
        TestCase<ArgumentConstructorTestCase>{{"--@user-name", "user_name"}, "ReplacesAtSignAndDash"},
        TestCase<ArgumentConstructorTestCase>{{"!!!important", "important"}, "ReplacesMultipleExclamations"},
        TestCase<ArgumentConstructorTestCase>{{"...hidden", "hidden"}, "ReplacesMultipleDots"},
        TestCase<ArgumentConstructorTestCase>{{"a b-c.d!e", "a_b_c_d_e"}, "ReplacesMultipleIllegalTypes"},

        // Leading numbers stripped
        TestCase<ArgumentConstructorTestCase>{{"123abc", "abc"}, "StripsLeadingNumbers"},
        TestCase<ArgumentConstructorTestCase>{{"99bottles", "bottles"}, "StripsLeadingDigits"},
        TestCase<ArgumentConstructorTestCase>{{"--123test", "test"}, "StripsDashesAndNumbers"},

        // Mixed leading non-alphas
        TestCase<ArgumentConstructorTestCase>{{"!@#hello", "hello"}, "StripsMixedSpecialChars"},
        TestCase<ArgumentConstructorTestCase>{{"   \t  name", "name"}, "StripsLeadingWhitespace"},
        TestCase<ArgumentConstructorTestCase>{{"-_-port", "port"}, "StripsMixedDashesAndUnderscores"},

        // No leading dashes
        TestCase<ArgumentConstructorTestCase>{{"my-option", "my_option"}, "ReplacesInternalDashNoLeading"},
        TestCase<ArgumentConstructorTestCase>{{"file.name", "file_name"}, "ReplacesInternalDot"},
        TestCase<ArgumentConstructorTestCase>{{"my option", "my_option"}, "ReplacesInternalSpace"},

        // Already valid - no change needed
        TestCase<ArgumentConstructorTestCase>{{"verbose", "verbose"}, "NoChangeAlreadyValid"},
        TestCase<ArgumentConstructorTestCase>{{"simple", "simple"}, "NoChangeSingleWord"},
        TestCase<ArgumentConstructorTestCase>{{"valid_name", "valid_name"}, "NoChangeWithUnderscores"},
        TestCase<ArgumentConstructorTestCase>{{"abc123", "abc123"}, "NoChangeAlphanumeric"},
        TestCase<ArgumentConstructorTestCase>{{"Option", "Option"}, "NoChangeUppercaseStart"},
        TestCase<ArgumentConstructorTestCase>{{"aBc123", "aBc123"}, "NoChangeMixedCase"},
        TestCase<ArgumentConstructorTestCase>{{"variable_name_123", "variable_name_123"}, "NoChangeComplexValid"},

        // Single character cases
        TestCase<ArgumentConstructorTestCase>{{"a", "a"}, "SingleAlphaCharUnchanged"},
        TestCase<ArgumentConstructorTestCase>{{"-a", "a"}, "SingleDashBeforeAlpha"},
        TestCase<ArgumentConstructorTestCase>{{"--a", "a"}, "DoubleDashBeforeAlpha"}
    ),
    TestCaseNamer<ArgumentConstructorTestCase>()
);

// ============================================================================
// Tests for Constructor Exception Cases
// ============================================================================

struct ArgumentConstructorThrowsTestCase {
    std::string input;
    std::string expected_error_substring;
};

class ArgumentConstructorThrowsTest : public ::testing::TestWithParam<TestCase<ArgumentConstructorThrowsTestCase>> {};

TEST_P(ArgumentConstructorThrowsTest, ConstructorThrowsOnInvalidName) {
    auto test_case = GetParam();

    EXPECT_THROW({
        try {
            Argument arg(test_case.value.input);
        }
        catch (const std::invalid_argument& e) {
            // Verify the error message contains expected information
            std::string error_msg = e.what();
            EXPECT_NE(error_msg.find(test_case.value.expected_error_substring), std::string::npos)
                << "Error message doesn't contain expected substring.\n"
                << "Expected substring: '" << test_case.value.expected_error_substring << "'\n"
                << "Actual error: '" << error_msg << "'";
            throw;
        }
        }, std::invalid_argument)
        << "Test: " << test_case.description
        << "\nInput: '" << test_case.value.input << "'";
}

INSTANTIATE_TEST_SUITE_P(
    ThrowsOnEmptyAfterCanonicalization,
    ArgumentConstructorThrowsTest,
    ::testing::Values(
        TestCase<ArgumentConstructorThrowsTestCase>{{"", "Original name: ''"}, "ThrowsOnEmptyString"},
        TestCase<ArgumentConstructorThrowsTestCase>{{"--", "Original name: '--'"}, "ThrowsOnOnlyDashes"},
        TestCase<ArgumentConstructorThrowsTestCase>{{"---", "Original name: '---'"}, "ThrowsOnOnlyTripleDash"},
        TestCase<ArgumentConstructorThrowsTestCase>{{"123", "Original name: '123'"}, "ThrowsOnOnlyNumbers"},
        TestCase<ArgumentConstructorThrowsTestCase>{{"@@@", "Original name: '@@@'"}, "ThrowsOnOnlyAtSigns"},
        TestCase<ArgumentConstructorThrowsTestCase>{{"!@#$%", "Original name: '!@#$%'"}, "ThrowsOnOnlySpecialChars"},
        TestCase<ArgumentConstructorThrowsTestCase>{{"   ", "Original name: '   '"}, "ThrowsOnOnlySpaces"},
        TestCase<ArgumentConstructorThrowsTestCase>{{"___", "Original name: '___'"}, "ThrowsOnOnlyUnderscores"},
        TestCase<ArgumentConstructorThrowsTestCase>{{"...", "Original name: '...'"}, "ThrowsOnOnlyDots"},
        TestCase<ArgumentConstructorThrowsTestCase>{{"!@#", "Original name: '!@#'"}, "ThrowsOnMixedSpecialChars"}
    ),
    TestCaseNamer<ArgumentConstructorThrowsTestCase>()
);