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

In the previous example, the different greetings were picked randomly on each generation with a uniform distribution. However, we can also supply a custom weighted distribution for the different greetings:

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
        std::map<calyx::String_t, double> {
            {"Hello", 5},
            {"Hi", 2},
            {"Hey", 2},
            {"Yo", 1}
        },
        errors
    );
    std::cout << grammar.generate(errors)->getText(grammar.getOptions()) << "\n";

    return 0;
}
```

In this case, the grammar will pick "Hello" 50% of the time, "Hi" and "Hey" 20% of the time, and "Yo" 10% of the time when greeting is expanded. 

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
        }
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

# Random sampling

By default, Calyx uses the Mersenne-Twister based random generator defined by [`std::mt19937`](https://cplusplus.com/reference/random/mt19937/) for random sampling. This type is assigned to a custom type, `calyx::Options::RandomGenerator_t`. A seed can be passed to a grammar in the constructor as an `unsigned long` with a strict mode. 

```c++
#include "calyx/grammar.h"

int main(int argc, char *argv[])
{
    calyx::Grammar grammar = calyx::Grammar(
        1234u, // seed value
        false  // strict mode
    );

    return 0;
}
```

If no seed is given, then a true (if available) random seed is chosen with [`std::random_device`](https://en.cppreference.com/w/cpp/numeric/random/random_device).

The random generator type cannot be updated by users, but it can be changed by maintainers to some other type.

### Options Random API 

The options class provides its own random API, backed by a `calyx::Options::RandomGenerator_t` source. This API allows users to generate both integers and real numbers of a variety of sizes, within some uniform ranges and bounds. Note that the random API is not guaranteed to produce the same result for the same seed across compilers or systems, due to differences in the random distribution mappings between C++ compiler implementations. 

For example, to generate a random `unsigned long` within the range [0, 10):

```c++
calyx::Options ops;

// generates a random unsigned long in the uniform range 0 (inclusive) to 10 (exclusive). 
std::cout << ops.randomInteger<unsigned long>(0, 10) << std::endl;
```

You can also generate random real numbers:

```c++
calyx::Options ops;

// generates a random float between 0 and 1 with uniform distribution 
std::cout << ops.randomReal<float>() << std::endl;
```

Note that if you input an invalid type to a random function, you will get a compiler error:

```c++
calyx::Options ops;

// ! Error C2338: T must be a real number type (eg double or float)
std::cout << ops.randomReal<int>() << std::endl;
```

See the [options header](/src/calyx/options.h) for more info on specific API functions. 

# Filters

Dot-notation is supported in template expressions, allowing you to call a variety of different processing functions on the string returned from a rule.

```c++
#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char *argv[])
{
    calyx::ErrorHolder errors;
    calyx::Grammar grammar = calyx::Grammar(
        [&](calyx::Grammar& g) {
            g.start("{hello.uppercase} there.", errors);
            if (errors) return;
            g.rule("hello", std::vector<calyx::String_t> { "Hello", "Hi" }, errors);
        }
    );

    std::cout << grammar.generate(errors)->getText(grammar.getOptions()) << "\n";
    // > HELLO there.
    
    return 0;
}
```

Multiple filters can also be chained onto the same rule, and are evaluated left to right:

```c++
#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char *argv[])
{
    calyx::ErrorHolder errors;
    calyx::Grammar grammar = calyx::Grammar(
        [&](calyx::Grammar& g) {
            g.start("{hello.uppercase.lowercase} there.", errors);
            if (errors) return;
            g.rule("hello", std::vector<calyx::String_t> { "Hello", "Hi" }, errors);
        }
    );

    std::cout << grammar.generate(errors)->getText(grammar.getOptions()) << "\n";
    // > hello there.
    
    return 0;
}
```

The full set of builtin filter functions is defined in the `calyx::filters` namespace in [`calyx/filters.h`](src/calyx/filters.h).

## Custom filters

Users can also define their own filters, and pass them to a grammar via a map of filter names to functions, or by passing name and function pairs one by one:

```c++
#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char *argv[])
{
    calyx::ErrorHolder errors;
    calyx::Grammar grammar = calyx::Grammar(
        [&](calyx::Grammar& g) {
            g.start("I love {animal.add_s}!", errors);
            if (errors) return;
            g.rule("animal", std::vector<calyx::String_t> { "cat", "dog" }, errors);
            if (errors) return;

            // Add the actual filter function to the grammar
            g.filter(
                "add_s",
                [](const calyx::String_t& input, calyx::Options& ops) {
                    // It is generally recommended to use string converters for these
                    // sorts of things...
                    return input + ops.fromString("s");
                }
            );
        }
    );

    std::cout << grammar.generate(errors)->getText(grammar.getOptions()) << "\n";
    // > I love dogs!
    
    return 0;
}
```

Filter functions must be of the form `(const String_t&, Options&) -> String_t`, but can be given as lambdas, functions, or any other valid `std::function`.

# Memoized functions

Rule expansions can be 'memoized' so that multiple references to the same rule return the same value. This is useful for picking a noun from a list and reusing it in multiple places within a text.

The `@` sigil is used to mark memoized rules. This evaluates the rule and stores it in memory the first time it's referenced. All subsequent references to the memoized rule use the same stored value.

```c++
#include <iostream>
#include "calyx/grammar.h"


int main(int argc, char* argv[])
{
    calyx::ErrorHolder errors;

    // Without memoization
    calyx::Grammar grammar = calyx::Grammar(
        [&](calyx::Grammar& g) {
            g.start("{name} <{name.lowercase}>", errors);
            if (errors) return;
            g.rule("name", std::vector<calyx::String_t> { "Daenerys", "Tyrion", "Jon" }, errors);
        }
    );

    for (int i = 0; i < 3; i++) 
    {
        std::cout << grammar.generate(errors)->getText(grammar.getOptions()) << "\n";
    }
    // > Tyrion <jon>
    // > Daenerys <tyrion>
    // > Jon <daenerys>

    // With memoization
    calyx::Grammar memoizedGrammar = calyx::Grammar(
        [&](calyx::Grammar& g) {
            g.start("{@name} <{@name.lowercase}>", errors);
            if (errors) return;
            g.rule("name", std::vector<calyx::String_t> { "Daenerys", "Tyrion", "Jon" }, errors);
        }
    );

    for (int i = 0; i < 3; i++)
    {
        std::cout << memoizedGrammar.generate(errors)->getText(memoizedGrammar.getOptions()) << "\n";
    }
    // > Tyrion <tyrion>
    // > Daenerys <daenerys>
    // > Jon <jon>
    
    return 0;
}
```

# Unique Rules

Rule expansions can be marked as 'unique', meaning that multiple references to the same rule always return a different value. This is useful for situations where the same result appearing twice would appear awkward and messy.

Unique rules are marked by the `$` sigil.

```c++
#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char* argv[])
{
    calyx::ErrorHolder errors;

    calyx::Grammar grammar = calyx::Grammar(
        [&](calyx::Grammar& g) {
            g.start("{$medal} {$medal} {$medal}", errors);
            if (errors) return;
            g.rule("medal", std::vector<calyx::String_t> { "Gold", "Silver", "Bronze" }, errors);
        }
    );

    std::cout << grammar.generate(errors)->getText(grammar.getOptions()) << "\n";
    // > Silver Gold Bronze
    return 0;
}
```

Note that if all options are exhausted, then the unique expansions cycle and begin producing the same values again, but in a different order.

# Dynamically Constructing Rules

Template expansions can be dynamically constructed at runtime by passing a context map of rules to the `generate()` method:

```c++
#include <iostream>
#include "calyx/grammar.h"

int main(int argc, char* argv[])
{
    calyx::ErrorHolder errors;

    calyx::Grammar grammar = calyx::Grammar(
        [&](calyx::Grammar& g) {
            g.start(
                std::vector<calyx::String_t> {
                    "Hi {username}!",
                    "Welcome back {username}...",
                    "Hola {username}"
                },
                errors
            );
        }
    );

    // read USERNAME environment variable 
    const char* cstr_username = std::getenv("USERNAME");
    std::string username = std::string(cstr_username == nullptr ? "" : cstr_username);
    
    // build dynamic context from environment variable
    const std::map<calyx::String_t, std::vector<calyx::String_t>> context = {
        { "username",  std::vector{ username }}
    };

    std::cout << grammar.generate(context, errors)->getText(grammar.getOptions()) << "\n";
    // > Hello TheDeathlyCow
    
    return 0;
}
```

# External File Formats

Calyx C++ does not currently support external file formats for loading Grammars. 