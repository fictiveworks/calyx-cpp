#include <catch2/catch_all.hpp>

#include <calyx/grammar.h>

using namespace calyx;

TEST_CASE("Can filter memoized rules")
{
    ErrorHolder errors;
    auto grammar = Grammar(
        1234u, // seed
        false, // strict
        [&](Grammar& g) {
            Options& ops = g.getOptions();
            g.start(ops.fromString("{@name.lowercase}"), errors);
            REQUIRE_FALSE(errors.hasError());
            g.rule(
                ops.fromString("name"),
                std::vector {
                    ops.fromString("Jewelsy")
                },
                errors
            );
            REQUIRE_FALSE(errors.hasError());
        }
    );
    REQUIRE_FALSE(errors.hasError());

    std::optional<Result> result = grammar.generate(errors);
    REQUIRE(result.has_value());
    REQUIRE_FALSE(errors.hasError());

    const String_t text = result->getText(grammar.getOptions());

    REQUIRE(text == grammar.getOptions().fromString("jewelsy"));
}