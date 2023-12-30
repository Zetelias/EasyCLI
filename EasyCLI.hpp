/**
 * @file EasyCLI.hpp
 * @author Elias Bort (zetelias@proton.me)
 * @brief A simple header-only library to make it easy to create a CLI by exposing a simple API with 2 structs and 1 class.
 * @remark This is a header-only library, so you don't need to compile anything. Just include this header file in your project and you're good to go!
 * @version 0.2
 * @date 2023-12-29
 *
 * @copyright GPLv3
 *
 */

#pragma once
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#define BINDFN(fn) static_cast<CommandOutput (*)(const CommandArguments &)>(fn)
#define COMMAND_FUNCTION(name) CommandOutput name(const CommandArguments &args)

/**
 * @brief A struct that contains the output of a command
 * @remark You, the user, needs to use this when you write your own commands
 *         as CommandOutput is returned by your command function
 *
 */
struct CommandOutput {
    std::string out;
    bool success;
};

/**
 * @brief  A struct that contains the parsed arguments of a command
 * @remark You, the user, needs to use this when you write your own commands
 *         as const CommandArguments &args is passed to your command function
 */
struct CommandArguments {
    std::string command;
    std::vector<std::string> arguments;
    std::vector<std::string> flags;

    /**
     * @brief Returns true if the flags vector contains the specified flag
     *
     * @param flag The flag to check for
     * @return true if the flags vector contains the specified flag
     * @return false if the flags vector does not contain the specified flag
     * @remark This is a convenience function that is equivalent to `std::find(arguments.begin(), arguments.end(), argument) != arguments.end();`
     * @remark The time complexity of this function is O(n), where n is the number of flags
     */
    bool flags_contains(const std::string &flag) const {
        return std::find(flags.begin(), flags.end(), flag) != flags.end();
    }

    /**
     * @brief Returns true if the arguments vector contains the specified flag
     *
     * @param flag The flag to check for
     * @return true if the arguments vector contains the specified flag
     * @return false if the arguments vector does not contain the specified flag
     * @remark This is a convenience function that is equivalent to `std::find(arguments.begin(), arguments.end(), argument) != arguments.end();`
     * @remark The time complexity of this function is O(n), where n is the number of arguments
     */
    bool arguments_contains(const std::string &argument) const {
        return std::find(arguments.begin(), arguments.end(), argument) != arguments.end();
    }
};

/**
 * @brief Parses a string into a CommandArguments struct
 *        For an input value of "echo hello world -oneline"
 *        The CommandArguments struct would look like this:
 *        {
 *            command: "echo",
 *            arguments: ["hello", "world"],
 *            flags: ["oneline"]
 *        }
 *
 * @remark You can use this directly, but it's simpler to use the variations of Execute{...} instead
 * @remark This uses Bash-like parsing, so "echo hello world" and "echo     hello     world" are equivalent
 * @param input The string to parse into a CommandArguments struct
 * @return CommandArguments The parsed CommandArguments struct
 */
inline CommandArguments ParseArgs(const std::string &input) {
    CommandArguments args;

    std::istringstream iss(input);
    std::string token;

    // Extract the command
    iss >> args.command;

    // Extract arguments and flags
    while (iss >> token) {
        if (token[0] == '-') {
            // Token is a flag
            args.flags.push_back(token.substr(1)); // Remove '-' and store the flag
        } else {
            // Token is an argument
            args.arguments.push_back(token);
        }
    }

    return args;
}

using CommandFunction = std::function<CommandOutput(const CommandArguments &)>;
using CommandMap = std::unordered_map<std::string, CommandFunction>;

/**
 * @brief  A class that makes it easy to create a CLI
 * @remark This class is not thread-safe (yet) but it should be fine for most use cases
 */
class EasyCLI {
  public:
    EasyCLI() {
    }

    EasyCLI(const CommandMap &commands) : commands(commands) {
    }

    /**
     * @brief Adds a command to the list of commands
     *
     * @param name the name of the command to add. it should not contain spaces or the command will be unreachable by user input
     * @param fn the callback function to call when the command is executed. Must be of type `std::function<CommandOutput(const CommandArguments &)>`
     */
    void RegisterCommand(const std::string &name, CommandFunction fn) {
        commands[name] = fn;
    }

    /**
     * @brief Adds a command to the list of commands
     *
     * @param name the name of the command to add. it should not contain spaces or the command will be unreachable by user input
     * @param fn the callback function to call when the command is executed. Must be of type `CommandOutput (*)(const CommandArguments &)`
     */
    void RegisterCommand(const std::string &name, CommandOutput (*fn)(const CommandArguments &)) {
        commands[name] = fn;
    }

    /**
     * @brief Adds a command to the list of commands
     *
     * @tparam T the type of the callback function to call when the command is executed. Must be of type `CommandOutput (*)(const CommandArguments &)`
     * @param name the name of the command to add. it should not contain spaces or the command will be unreachable by user input
     * @param c the callback function to call when the command is executed. Must be of type `CommandOutput (*)(const CommandArguments &)`
     */
    template <typename T> void RegisterCommand(const std::string &name, T c) {
        commands[name] = BINDFN(c);
    }

    /**
     * @brief Executes a command from user input and returns the output
     *
     * @remark This is the most basic way to execute a command, but it's not the most convenient.
     * @param input The user input to parse and execute
     * @return CommandOutput The output of the command or an error output if the command failed.
     */
    CommandOutput Execute(const std::string &input) {
        CommandArguments args = ParseArgs(input);
        if (commands.find(args.command) != commands.end()) {
            CommandOutput out = commands[args.command](args);
            return out;
        }
        const std::string error = "Unknown command: \"" + args.command + "\"";
        return CommandOutput{error, false};
    }

    /**
     * @brief Executes a command from user input and sends error output to a stream (std::cerr by default)
     *
     * @param input The user input to parse and execute
     * @param error_stream The stream to send error output to. std::cerr by default
     * @return CommandOutput The output of the command or an error output if the command failed.
     */
    CommandOutput ExecuteWithErrStream(const std::string &input, std::ostream &error_stream = std::cerr) {
        CommandArguments args = ParseArgs(input);
        if (commands.find(args.command) != commands.end()) {
            CommandOutput out = commands[args.command](args);
            if (out.success) {
                return out;
            } else {
                error_stream << out.out;
            }
        }
        const std::string error = "Unknown command: \"" + args.command + "\"";
        error_stream << error;
        return CommandOutput{error, false};
    }

    /**
     * @brief Executes a command from user input and sends output to a stream (std::cout by default) and error output to another stream (std::cerr by default)
     *
     * @param input The user input to parse and execute
     * @param output_stream The stream to send output to. std::cout by default
     * @param error_stream The stream to send error output to. std::cerr by default
     * @return CommandOutput the output of the command or an error output if the command failed.
     */
    CommandOutput ExecuteIntoStream(const std::string &input, std::ostream &output_stream = std::cout, std::ostream &error_stream = std::cerr) {
        CommandArguments args = ParseArgs(input);
        if (commands.find(args.command) != commands.end()) {
            CommandOutput out = commands[args.command](args);
            if (out.success) {
                output_stream << out.out << std::endl;
                return out;
            } else {
                error_stream << out.out << std::endl;
            }
        }
        const std::string error = "Unknown command: \"" + args.command + "\"";
        return CommandOutput{error, false};
    }

    /**
     * @brief Execute a command without returning the output
     *
     * @param input The user input to parse and execute
     */
    void ExecuteVoid(const std::string &input) {
        CommandArguments args = ParseArgs(input);
        if (commands.find(args.command) != commands.end()) {
            commands[args.command](args);
        }
    }

    /**
     * @brief Execute a command and modifies a string that is passed by reference with the output of the command
     *
     * @param input The user input to parse and execute
     * @param output The string to modify with the output of the command
     */
    void ExecuteVoidIntoString(const std::string &input, std::string &output) {
        CommandArguments args = ParseArgs(input);
        if (commands.find(args.command) != commands.end()) {
            CommandOutput out = commands[args.command](args);
            output = out.out;
        }
    }

    /**
     * @brief Executes a command from user input and sends error output to a stream (std::cerr by default)
     *
     * @param input The user input to parse and execute
     * @param error_stream The stream to send error output to. std::cerr by default
     */
    void ExecuteVoidWithErrStream(const std::string &input, std::ostream &error_stream = std::cerr) {
        CommandArguments args = ParseArgs(input);
        if (commands.find(args.command) != commands.end()) {
            CommandOutput out = commands[args.command](args);
            if (!out.success) {
                error_stream << out.out << std::endl;
            }
        }
        const std::string error = "Unknown command: \"" + args.command + "\"";
        error_stream << error;
    }

    /**
     * @brief Executes a command from user input and sends output to a stream (std::cout by default) and error output to another stream (std::cerr by default)
     *
     * @param input The user input to parse and execute
     * @param output_stream The stream to send output to. std::cout by default
     * @param error_stream The stream to send error output to. std::cerr by default
     */
    void ExecuteVoidIntoStream(const std::string &input, std::ostream &output_stream = std::cout, std::ostream &error_stream = std::cerr) {
        CommandArguments args = ParseArgs(input);
        if (commands.find(args.command) != commands.end()) {
            CommandOutput output = commands[args.command](args);
            if (output.success) {
                output_stream << output.out << std::endl;
            } else {
                error_stream << output.out << std::endl;
            }
        }
    }

    /**
     * @brief Gets a list of all the commands registered as a vector of strings
     *
     * @return A vector of strings containing all the commands
     */
    std::vector<std::string> GetCommandsString() const {
        std::vector<std::string> command_list;
        for (const auto &command : commands) {
            command_list.push_back(command.first);
        }
        return command_list;
    }

    /**
     * @brief gets a list of all the commands registered as a vector of CommandFunction
     *
     * @return a vector of CommandFunction containing all the commands
     */
    std::vector<CommandFunction> GetCommands() const {
        std::vector<CommandFunction> command_list;
        for (const auto &command : commands) {
            command_list.push_back(command.second);
        }
        return command_list;
    }

    /**
     * @brief Sugar for ExecuteVoidIntoStream that takes argv as input and not a string
     *
     * @param argv the argv array from main()
     */
    void Run(char **argv, bool cout_output = true, bool cerr_output = true) {
        if (argv[1] != nullptr) {
            std::string input = "";
            argv++; // Skip the first argument
            while (*argv != nullptr) {
                input += *argv;
                input += " ";
                argv++;
            }
            ExecuteVoidIntoStream(input);
        } else {
            std::cerr << "Available commands:" << std::endl;
            for (const auto &command : GetCommandsString()) {
                std::cerr << "\t- " << command << std::endl;
            }
        }
    }

  protected:
    CommandMap commands;
};