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

TEST(EasyCliTest, MultiplyTest) {
    EasyCLI cli;
    cli.RegisterCommand("multiply", multiply);
    CommandOutput out = cli.Execute("multiply 2 3");
    EXPECT_EQ(out.out, "6");
    EXPECT_TRUE(out.success);
}

COMMAND_FUNCTION(echo) {
    CommandOutput out;
    try {
        for (auto &arg : args.arguments) {
            out.out += arg + " ";
        }
        out.out.pop_back();
        out.success = true;
    } catch (std::exception &e) {
        out.out = e.what();
        out.success = false;
    }
    return out;
}

TEST(EasyCliTest, EchoTest) {
    EasyCLI cli;
    cli.RegisterCommand("echo", echo);
    CommandOutput out = cli.Execute("echo Hello World");
    EXPECT_EQ(out.out, "Hello World");
    EXPECT_TRUE(out.success);
}

COMMAND_FUNCTION(greet) {
    CommandOutput out;
    try {
        out.out = "Hello, " + args.arguments[0] + "!";
        out.success = true;
    } catch (std::exception &e) {
        out.out = e.what();
        out.success = false;
    }
    return out;
}

TEST(EasyCliTest, GreetTest) {
    EasyCLI cli;
    cli.RegisterCommand("greet", greet);
    CommandOutput out = cli.Execute("greet World");
    EXPECT_EQ(out.out, "Hello, World!");
    EXPECT_TRUE(out.success);
}

COMMAND_FUNCTION(flag) {
    CommandOutput out;
    try {
        if (args.flags_contains("flag")) {
            out.out = "Flag is set";
        } else {
            out.out = "Flag is not set";
        }
    } catch (std::exception &e) {
        out.out = e.what();
        out.success = false;
    }
    return out;
}

TEST(EasyCliTest, FlagTest) {
    EasyCLI cli;
    cli.RegisterCommand("flag", flag);
    CommandOutput out = cli.Execute("flag -flag");
    EXPECT_EQ(out.out, "Flag is set");
    EXPECT_TRUE(out.success);
}

TEST(EasyCliTest, GetCommandsStringTest) {
    EasyCLI cli;
    cli.RegisterCommand("greet", greet);
    cli.RegisterCommand("echo", echo);
    cli.RegisterCommand("multiply", multiply);
    std::vector<std::string> commands = cli.GetCommandsString();
    bool contains_echo = std::find(commands.begin(), commands.end(), "echo") != commands.end();
    bool contains_greet = std::find(commands.begin(), commands.end(), "greet") != commands.end();
    bool contains_multiply = std::find(commands.begin(), commands.end(), "multiply") != commands.end();
    EXPECT_TRUE(contains_echo);
    EXPECT_TRUE(contains_greet);
    EXPECT_TRUE(contains_multiply);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}