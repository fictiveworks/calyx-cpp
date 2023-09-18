#include <iostream>
#include <catch2/catch_all.hpp>

#include <include/calyx.h>
#include <registry.h>

using namespace calyx;

TEST_CASE("Evaluate start rule")
{
    Registry registry;
    ErrorHolder errs;
    const Options& ops = registry.getOptions();
    String_t atom = ops.fromString("atom");
    String_t start = ops.fromString("start");

    registry.defineRule(
        start,
        std::vector { atom },
        errs
    );

    REQUIRE_FALSE(errs.hasError());

    std::optional<Expansion> exp = registry.evaluate(start, errs);

    REQUIRE_FALSE(errs.hasError());
    REQUIRE(exp.has_value());
    REQUIRE(exp->getSymbol() == Exp::RESULT);
    REQUIRE(exp->flatten(ops) == atom);
}

TEST_CASE("Evaluate recursive rules")
{
    Registry registry;
    ErrorHolder errs;
    const Options& ops = registry.getOptions();
    String_t start = ops.fromString("start");
    String_t prod = ops.fromString("{atom}");
    String_t atom = ops.fromString("atom");

    registry.defineRule(start, std::vector { prod }, errs);
    REQUIRE_FALSE(errs.hasError());
    registry.defineRule(atom, std::vector { atom }, errs);
    REQUIRE_FALSE(errs.hasError());

    std::optional<Expansion> exp = registry.evaluate(start, errs);

    REQUIRE_FALSE(errs.hasError());
    REQUIRE(exp.has_value());
    REQUIRE(exp->getSymbol() == Exp::RESULT);
    REQUIRE(exp->flatten(ops) == atom);
}