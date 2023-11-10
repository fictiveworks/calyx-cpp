#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <catch2/catch_session.hpp>

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

    return Catch::Session().run(argc, argv);
}