#include <gtest/gtest.h>

#include "arg_parse/argument.h"
#include "arg_parse/argument_parser.h"


class ArgumentParameterizedTest : public ::testing::TestWithParam<Argument> {};

TEST_P(ArgumentParameterizedTest, CanConstructWithParameters) {
    auto expected = GetParam();
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
        Argument{ "--input", "Input file", 1, true },
        Argument{ "--output", "Output file", 1, false },
        Argument{ "--verbose", "Verbose mode", 0, false },
        Argument{ "--config", "Config files", 3, true },
        Argument{ "", "Empty name", 0, false }
    )
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