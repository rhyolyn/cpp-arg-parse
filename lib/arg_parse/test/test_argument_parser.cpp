#include <gtest/gtest.h>

#include "arg_parse/argument.h"
#include "arg_parse/argument_parser.h"


TEST(ArgParserTest, CanInstantiate) {
    ArgumentParser parser;
    SUCCEED();
}

TEST(ArgParserTest, CanAddArgument) {
    ArgumentParser parser;
    Argument arg{ "input", "Input file", 1, true };  // Fixed: added num_options parameter
    parser.add_argument(arg);
    SUCCEED();
}