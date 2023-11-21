# Calyx: C++

C++ port of the Calyx generative grammar engine.

## Development

### Install build tools

See the [Xmake installation guide](https://xmake.io/#/guide/installation).

### Compile and link

```bash
xmake build -y
```

(The `-y` is to confirm installation of Catch2 for testing)

### Run test executable

Calyx C++ is tested using Catch2. 

```bash
xmake run test
```

### Build for release

TBD

### Generate Visual Studio and Rider IDE files

```bash
xmake project -k vsxmake -m "debug,release"
```

## License

Will be licensed MIT when open sourced.

# Usage and Examples

The primary file to include in projects that use calyx is `calyx/grammar.h`. All headers in the `calyx` folder are considered to be part of the Calyx public API other files and headers are considered internal and should not be relied on to remain stable. 

First, a few basics on some weird things that Calyx does at its core:

### Error Handling

Calyx does not make use of exceptions for errors. Instead, a `calyx::ErrorHolder` object is passed to all methods that may produce errors. If an error is generated, then a message will be placed into the error holder and further processing is cancelled. For potentially error-producing methods that return a value, instead of returning a regular value, they will return `std::optional` types or a nullable pointer, if relevant. The optional will have a value if and only if the error holder does not have an error. If the error holder has an error, then pointer type returns will definitely be null, though they may still be null in other cases, depending on the method contract. 

Error holders must be passed by reference, and users can check if an error has occured with `calyx::ErrorHolder#hasError()` or using the bool operator. If an error is present, the error message can be retrieved with `calyx::ErrorHolder#getMessage()`. If no message is present, then this will return an empty string. 

### Strings 

Calyx is able to switch its default string type by redefining the `calyx::String_t` custom type. This is so that the default string type used by calyx can be converted from the standard C++ `std::string` and Unreal Engine's `FString` without much hassle. An interface is provided for converting to and from that default and unknown `String_t` type to a concrete `std::string` with the `calyx::StringCoverter` interface. This interface is implemented in `calyx::Options`, so that users can set their own string converters if they want to. The default string coverter (ie, the string converter for the `String_t` type), can be accessed with the macro `calyx::DEFAULT_STRING_CONVERTER`.

The only requirement for a custom string type is that it must be mutable, and allow for concatenation with the `+` and `+=` operators.

## Generating basics

Include the library and construct a `calyx::Grammar` to construct a set of rules and generate text.

```c++
#include <iostream>
#include <calyx/grammar.h>

int main()
{
    calyx::Grammar grammar = calyx::Grammar();
    calyx::ErrorHolder errors;
    grammar.start("Hello world.", errors);

    // Always make sure to check your errors!
    if (errors.hasError()) {
        std::cout << "Error defining grammar: " << errors.getMessage() << std::endl;
        return 1;
    }
    
    return 0;
}
```

Once the grammar is constructed, call the `generate()` method to retrieve a randomly generated result. This result contains a tree representation of the generation output, but it can be and converted that tree into text with `getText()`:

```c++
#include <iostream>
#include <calyx/grammar.h>

int main()
{
    // ... Grammar construction from previous example ...

    // You can reuse the same error holder if no errors were generated!
    std::optional<Result> tree = grammar.generate(errors);
    
    if (!tree || errors) { // using the bool operator to check errors
        std::cout << "Error generating text: " << errors.getMessage() << std::endl;
        return 1;
    }

    // Options provides for string conversion, which is needed in the 
    // tree flattening to build the final text.
    calyx::Options& options = grammar.getOptions();

    std::cout << tree->getText(options) << std::endl;
    // > "Hello world."
    
    return 0;
}
```

Obviously, this hardcoded sentence isn’t very interesting by itself. Possible variations can be added to the text by adding additional rules which provide a named set of text strings. The rule delimiter syntax (`{}`) can be used to substitute the generated content of other rules.

```c++
#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char *argv[])
{

    calyx::Grammar grammar = calyx::Grammar();
    calyx::ErrorHolder errors;
    grammar.start("{greeting} world.", errors);
    grammar.rule(
        "greeting",
        std::vector<calyx::String_t> { "Hello", "Hi", "Hey", "Yo"},
        errors
    );

    
    return 0;
}
```
(Note that error handling here is ignored for the sake of brevity)


Each time `generate()` runs, it evaluates the tree and randomly selects variations of rules to construct a resulting string.

```c++
#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char *argv[])
{
    // ... Grammar construction from previous example ...
    
    calyx::Options& options = grammar.getOptions();
    std::cout << grammar.generate(errors)->getText(options) << "\n";
    // > "Hey world."
    std::cout << grammar.generate(errors)->getText(options) << "\n";
    // > "Hi world."
    std::cout << grammar.generate(errors)->getText(options) << "\n";
    // > "Yo world."
    std::cout << grammar.generate(errors)->getText(options) << "\n";
    // > "Hello world."

    
    return 0;
}
```

By convention, the `start` rule specifies the default starting point for generating the final text. You can start from any other named rule by passing it explicitly to the generate method.

```c++
#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char *argv[])
{
    calyx::Grammar grammar = calyx::Grammar();
    calyx::ErrorHolder errors;
    grammar.rule(
        "hello",
        "Hello world.",
        errors
    );

    calyx::Options& options = grammar.getOptions();
    std::cout << grammar.generate("hello", errors)->getText(options) << "\n";
    // > "Hello world."

    return 0
}
```

## Callback initialization

As an alternative to specifying rules line by line, you can also construct rules by passing an initialization callback to the Grammar constructor. This is particularly useful for initializing grammars in static or class variables.

```c++
#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char *argv[])
{
    calyx::ErrorHolder errors;
    calyx::Grammar grammar = calyx::Grammar(
        // Capture errors object by reference
        [&](calyx::Grammar& g) {
            g.rule("hello", "Hello world.", errors);
        },
        false
    );

    calyx::Options& options = grammar.getOptions();
    std::cout << grammar.generate("hello", errors)->getText(options) << "\n";
    // > "Hello world."

    return 0
}
```

## Template Expressions

Basic rule substitution uses single curly brackets as delimiters for template expressions:

```c++

#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char *argv[])
{
    calyx::ErrorHolder errors;
    calyx::Grammar grammar = calyx::Grammar(
        [&](calyx::Grammar& g) {
            g.start("{colour} {fruit}", errors);
            if (errors) return;
            g.rule("colour", std::vector<calyx::String_t> { "red", "green", "yellow" }, errors);
            if (errors) return;
            g.rule("fruit", std::vector<calyx::String_t> { "apple", "pear", "tomato" }, errors);
        },
        false
    );

    calyx::Options& options = grammar.getOptions();

    for (int i = 0; i < 6; i++)
    {
        std::cout << grammar.generate(errors)->getText(options) << "\n";
    }
    // > "yellow pear"
    // > "red apple"
    // > "green tomato"
    // > "red pear"
    // > "yellow tomato"
    // > "green apple"

    return 0;
}
```