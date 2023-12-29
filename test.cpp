#include "EasyCLI.hpp"
#include <gtest/gtest.h>

COMMAND_FUNCTION(multiply) {
    CommandOutput out;
    try {
        int a = std::stoi(args.arguments[0]);
        int b = std::stoi(args.arguments[1]);
        out.out = std::to_string(a * b);
        out.success = true;
    } catch (std::exception &e) {
        out.out = e.what();
        out.success = false;
    }
    return out;
}

TEST(EasyCLITest, MultiplyCommand) {
    EasyCLI cli;
    cli.RegisterCommand("multiply", multiply);
    CommandOutput out = cli.Execute("multiply 2 3");
    ASSERT_TRUE(out.success);
    ASSERT_EQ(out.out, "6");
}

TEST(EasyCLITest, MultiplyCommandFail) {
    EasyCLI cli;
    cli.RegisterCommand("multiply", multiply);
    CommandOutput out = cli.Execute("multiply 2 a");
    ASSERT_FALSE(out.success);
    ASSERT_EQ(out.out, "stoi");
}

TEST(EasyCLITest, GetCommands) {
    EasyCLI cli;
    cli.RegisterCommand("multiply", multiply);
    auto commands = cli.GetCommandsString();
    ASSERT_EQ(commands.size(), 1);
    ASSERT_EQ(commands[0], "multiply");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}