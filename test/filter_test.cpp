#include <catch2/catch_all.hpp>

#include "include/calyx.h"

using namespace calyx;

TEST_CASE("Upper case filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t upper = filters::uppercase(base, ops);

    REQUIRE(upper == ops.fromString("STRING"));
}

TEST_CASE("Lower case filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t lower = filters::lowercase(base, ops);

    REQUIRE(lower == ops.fromString("string"));
}

TEST_CASE("Emphasis filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t filtered = filters::emphasis(base, ops);

    REQUIRE(filtered == ops.fromString("*String*"));
}

TEST_CASE("Length filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t filtered = filters::length(base, ops);

    REQUIRE(filtered == ops.fromString("6"));
}

TEST_CASE("Can define custom filter as lambda")
{
    // very similar to the test before, but with a function ptr instead of a lambda
    Grammar grammar;
    Options& options = grammar.getOptions();

    grammar.filters(
        std::map<String_t, filters::Filter_t> {
            {
                options.fromString("backwards"),
                [](const String_t& input, const Options& options) -> String_t {
                    std::string str = options.toString(input);

                    std::ranges::reverse(str.begin(), str.end());

                    return options.fromString(str);
                }
            }
        }
    );

    ErrorHolder errs;
    grammar.rule(
        options.fromString("start"),
        std::vector { options.fromString("{prod.backwards}") },
        errs
    );
    REQUIRE_FALSE(errs.hasError());
    grammar.rule(
        options.fromString("prod"),
        std::vector { options.fromString("god") },
        errs
    );
    REQUIRE_FALSE(errs.hasError());

    std::optional<Result> result = grammar.generate(errs);
    REQUIRE(result.has_value());
    REQUIRE_FALSE(errs.hasError());

    REQUIRE(result->getText(options) == options.fromString("dog"));
}
