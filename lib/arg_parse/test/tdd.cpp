#include <gtest/gtest.h>
#include <arg_parse/argument_parser.h>
#include "test_utils.h"

TEST(ParserTest, ParsesTokens) {
    ArgumentParser parser;
    MockArgv mock({ "program", "--ip" });

    parser.parse(mock.getArgc(), mock.getArgv());

    const auto& parsed = parser.get_parsed_arguments();  // Changed to const auto&
    ASSERT_EQ(parsed.size(), 1);
    EXPECT_EQ(parsed[0].get_name(), "ip");  // Canonicalized: leading dashes removed
}

TEST(ParserTest, ArgumentNameInAvailableArguments) {
    ArgumentParser parser;
    AvailableArgument arg("--ip", "IP address", 1);

    parser.add_argument(arg);

    const auto& available = parser.get_available_arguments();  // Changed to const auto&
    ASSERT_EQ(available.size(), 1);
    EXPECT_EQ(available[0].get_name(), "ip");  // Canonicalized: leading dashes removed
}