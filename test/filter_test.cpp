#include <catch2/catch_all.hpp>

#include "include/calyx.h"

using namespace calyx;

TEST_CASE("Upper case filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t upper = BuiltinFilters::uppercase(base, ops);

    REQUIRE(upper == ops.fromString("STRING"));
}

TEST_CASE("Lower case filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t lower = BuiltinFilters::lowercase(base, ops);

    REQUIRE(lower == ops.fromString("string"));
}

TEST_CASE("Emphasis filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t filtered = BuiltinFilters::emphasis(base, ops);

    REQUIRE(filtered == ops.fromString("*String*"));
}

TEST_CASE("Length filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t filtered = BuiltinFilters::length(base, ops);

    REQUIRE(filtered == ops.fromString("6"));
}

TEST_CASE("Can define custom filter")
{
    Registry registry;
    Options& options = registry.getOptions();

    registry.addFilter(
        options.fromString("backwards"),
        [](const String_t& input, const Options& options) -> String_t {
            std::string str = options.toString(input);

            std::ranges::reverse(str.begin(), str.end());

            return options.fromString(str);
        }
    );

    ErrorHolder errs;
    registry.defineRule(
        options.fromString("start"),
        std::vector { options.fromString("{prod.backwards}") },
        errs
    );
    REQUIRE_FALSE(errs.hasError());
    registry.defineRule(
        options.fromString("prod"),
        std::vector { options.fromString("god") },
        errs
    );
    REQUIRE_FALSE(errs.hasError());

    std::optional<Expansion> s = registry.evaluate(options.fromString("start"), errs);
    REQUIRE(s.has_value());
    REQUIRE_FALSE(errs.hasError());

    REQUIRE(s->flatten(options) == options.fromString("dog"));
}