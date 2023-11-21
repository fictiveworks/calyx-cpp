#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <catch2/catch_session.hpp>

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
            g.rule("fruit", std::vector<calyx::String_t> { "apply", "pear", "tomato" }, errors);
        },
        false
    );

    calyx::Options& options = grammar.getOptions();

    for (int i = 0; i < 6; i++)
    {
        std::cout << grammar.generate("hello", errors)->getText(options) << "\n";
    }
    // > "yellow pear"
    // > "red apple"
    // > "green tomato"
    // > "red pear"
    // > "yellow tomato"
    // > "green apple"

    return Catch::Session().run(argc, argv);
}