﻿#include <catch2/catch_all.hpp>

#include <calyx/grammar.h>
#include <registry.h>

using namespace calyx;

TEST_CASE("Strict options returned error with unknown rule")
{
    Registry registry(Options(true));
    Options& ops = registry.getOptions();
    String_t start = ops.fromString("start");
    
    ErrorHolder errs;
    std::optional<Expansion> exp = registry.evaluate(start, errs);

    REQUIRE(errs.hasError());
    REQUIRE_FALSE(exp.has_value());
    REQUIRE(errs.getMessage() == Errors::undefinedRule(start, ops));
}

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

TEST_CASE("Evaluate rules with dynamic context")
{
    Registry registry(std::make_shared<Options>(true));
    const Options& ops = registry.getOptions();


    String_t start = ops.fromString("start");
    String_t atom = ops.fromString("atom");
    ErrorHolder errs;

    // define "start" -> "{atom}", without definition for atom
    registry.defineRule(start, std::vector { ops.fromString("{atom}") }, errs);
    REQUIRE_FALSE(errs.hasError());

    // add definition for atom in dynamic context
    std::map<String_t, std::vector<String_t>> context = {
        { atom, std::vector { atom } }
    };
    std::optional<Expansion> exp = registry.evaluate(start, context, errs);

    REQUIRE_FALSE(errs.hasError());
    REQUIRE(exp.has_value());
    REQUIRE(exp->getSymbol() == Exp::RESULT);
    REQUIRE(exp->flatten(ops) == atom);
}

TEST_CASE("Evaluate rules only from dynamic context")
{
    Registry registry(std::make_shared<Options>(true));
    Options& ops = registry.getOptions();
    
    String_t start = ops.fromString("start");
    String_t prod = ops.fromString("{atom}");
    String_t atom = ops.fromString("atom");
    ErrorHolder errs;

    std::map<String_t, std::vector<String_t>> context = {
        { start, std::vector { prod } },
        { atom, std::vector { atom } }
    };
    std::optional<Expansion> exp = registry.evaluate(start, context, errs);

    REQUIRE_FALSE(errs.hasError());
    REQUIRE(exp.has_value());
    REQUIRE(exp->getSymbol() == Exp::RESULT);
    REQUIRE(exp->flatten(ops) == atom);
}

TEST_CASE("Memoized rules return identical expression")
{
    Registry registry(std::make_shared<Options>(556677, true));
    const Options& ops = registry.getOptions();

    String_t start = ops.fromString("start");
    String_t prod = ops.fromString("{@atom}{@atom}{@atom}");
    String_t atom = ops.fromString("atom");
    ErrorHolder errs;

    registry.defineRule(start, std::vector { prod }, errs);
    REQUIRE_FALSE(errs.hasError()); 
    registry.defineRule(atom, std::vector { ops.fromString("~"), ops.fromString(":"), ops.fromString("%") }, errs);
    REQUIRE_FALSE(errs.hasError());
    
    std::optional<Expansion> exp = registry.evaluate(start, errs);
    
    REQUIRE_FALSE(errs);
    REQUIRE(exp.has_value());
    REQUIRE(exp->getSymbol() == Exp::RESULT);
    REQUIRE(exp->flatten(ops) == ops.fromString(":::"));
}
