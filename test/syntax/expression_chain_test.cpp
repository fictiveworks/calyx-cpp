#include <catch2/catch_all.hpp>

#include <include/calyx.h>

using namespace calyx;

TEST_CASE("Can apply filters")
{
    Registry registry;
    Options& ops = registry.getOptions();
    ErrorHolder errs;

    registry.defineRule(ops.fromString("start"), { "greekLetter.uppercase" }, errs);
    REQUIRE_FALSE(errs.hasError());
    registry.defineRule(ops.fromString("greekLetter"), { "alpha" }, errs);
    REQUIRE_FALSE(errs.hasError());

    std::optional<Expansion> exp = registry.evaluate(ops.fromString("start"), errs);
    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errs.hasError());

    REQUIRE(exp->getSymbol() == Exp::RESULT);
    REQUIRE(exp->flatten(ops) == ops.fromString("ALPHA"));
}