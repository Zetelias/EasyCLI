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

    // 3. RUN!
    cli.Run(argv);
}

```
> Note: this is not the only way of using EasyCLI, but it's enough in most cases

## Documentation
The documentation as a pdf is available [here](https://cdn.discordapp.com/attachments/1180670034121592954/1190138435390935040/EasyCLIDocumentation.pdf?ex=65a0b5ca&is=658e40ca&hm=8e25b6a373cba6a93109c8c7b2e930f7e8742b6b6be9b5d5c49def8cb84fdd94&). If it's not enough then ask me questions on my discord (it is "zetelias")

## Contributing
I'd be happy to merge your pull requests or even take new maintainers! Just make sure your code compiles and doesn't break the guideline of simplicity and usability.