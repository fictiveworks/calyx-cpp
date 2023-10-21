#include <catch2/catch_all.hpp>

#include <include/calyx.h>

using namespace calyx;

TEST_CASE("Can apply filters")
{
    Grammar grammar = Grammar();
    Options& ops = grammar.getOptions();
    ErrorHolder errs;

    grammar.rule(ops.fromString("start"), "greekLetter.uppercase", errs);
    REQUIRE_FALSE(errs.hasError());
    grammar.rule(ops.fromString("greekLetter"), "alpha", errs);
    REQUIRE_FALSE(errs.hasError());

    std::optional<Result> exp = grammar.generate(errs);
    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errs.hasError());

    REQUIRE(exp->getTree().getSymbol() == Exp::RESULT);
    REQUIRE(exp->getText(ops) == ops.fromString("ALPHA"));
}