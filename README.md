# EasyCLI
 A lightweight C++ header only library that makes it easy to create CLI's

## Installation
As this is a header only library, you simply need to download "EasyCLI.hpp", drop it in your project files and include it with #include "EasyCLI.hpp".

## Usage
```cpp
#include "EasyCLI.hpp"
#include <string>
using namespace std;

// This is a macro that declares a function with:
//  - symbol name "multiply"
//  - return type of CommandOutput
//  - const CommandArgs& as input
// That is the type of function used in the CLI
COMMAND_FUNCTION(multiply) {
    // At the start of your function,
    // declare your returned CommandOuput
    CommandOutput output;

    try {
        // use stoi to convert string to int
        int a = stoi(args.arguments[0]);
        int b = stoi(args.arguments[1]);

        // .out is the output of the command,
        // .success is whether the command succeeded
        //  CommandOutput doubles as an error message
        output.out = to_string(a * b);
        output.success = true;
    } 

    catch (const exception &e) {
        // Because CommandOutput doubles as an error message
        // in case of error, set .success to false 
        // and .out to the error message
        output.out = "Error: " + string(e.what());
        output.success = false;
    }
    
    return output;
}

int main(int argc, char **argv) {
    // 1. Create an EasyCLI object
    EasyCLI cli;

    // 2. Register commands
    cli.RegisterCommand("multiply", multiply);

    // 3. RUN! This parses a command from argv and then tries to execute it.
    //    If your functions produce errors or the command the user is calling doesn't exist,
    //    it will output to std::cerr.
    //    Else if everything goes well, it will output a message to std::cout.
    cli.Run(argv);
}

```
> Note: this is not the only way of using EasyCLI, but it's enough in most cases

## Documentation
The documentation as a pdf is available for each release in the "release" tab.
If it's not enough then ask me questions on my discord (it is "zetelias")

> Note: If you're on linux and have everything needed to generate a .pdf from a Doxyfile, use the `generate_pdf.py` script.

## Testing
This uses google test and CTest for testing. Tests are written in test.cpp. If you want to contribute code make sure to do tests for it. Right now the tests don't cover as much as i'd want it to cover so more would be appreciated.

## Contributing
I'd be happy to merge your pull requests or even take new maintainers! Just make sure your code compiles and doesn't break the guideline of simplicity and usability.

## TODO's
With no particular order:
- Change the parser and data types to allow flags and arguments to carry data, like if the command was `my_command bits=64` then `args.arguments["bits"] == "64"`
- As i've said more tests would be appreciated
- Polish the documentation
- Make function names clearer

