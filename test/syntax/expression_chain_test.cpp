#include <catch2/catch_all.hpp>

#include <calyx/grammar.h>
#include <iostream>

using namespace calyx;

TEST_CASE("Can apply filters")
{
    Grammar grammar = Grammar();
    Options& ops = grammar.getOptions();
    ErrorHolder errs;
    
    grammar.rule(ops.fromString("start"), "{greekLetter.uppercase}", errs);
    REQUIRE_FALSE(errs.hasError());
    grammar.rule(ops.fromString("greekLetter"), "alpha", errs);
    REQUIRE_FALSE(errs.hasError());

    std::optional<Result> exp = grammar.generate(errs);
    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errs.hasError());

    REQUIRE(exp->getTree().getSymbol() == Exp::RESULT);
    REQUIRE(exp->getText(ops) == ops.fromString("ALPHA"));
}

TEST_CASE("Can apply chained filters")
{
    Grammar grammar = Grammar();
    Options& ops = grammar.getOptions();
    ErrorHolder errs;

    grammar.rule(ops.fromString("start"), "{greekLetter.uppercase.emphasis}", errs);
    REQUIRE_FALSE(errs.hasError());
    grammar.rule(ops.fromString("greekLetter"), "alpha", errs);
    REQUIRE_FALSE(errs.hasError());

    std::optional<Result> exp = grammar.generate(errs);
    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errs.hasError());

    REQUIRE(exp->getTree().getSymbol() == Exp::RESULT);
    REQUIRE(exp->getText(ops) == ops.fromString("*ALPHA*"));
}

TEST_CASE("Undefined filter produces an error")
{
    Grammar grammar = Grammar();
    Options& ops = grammar.getOptions();
    ErrorHolder errs;

    grammar.rule(ops.fromString("start"), "{greekLetter.notafilter}", errs);
    REQUIRE_FALSE(errs.hasError());
    grammar.rule(ops.fromString("greekLetter"), "alpha", errs);
    REQUIRE_FALSE(errs.hasError());

    std::optional<Result> exp = grammar.generate(errs);
    REQUIRE_FALSE(exp.has_value());
    REQUIRE(errs.hasError());
    REQUIRE(errs.getMessage() == Errors::undefinedFilter(ops.fromString("notafilter"), ops));
}