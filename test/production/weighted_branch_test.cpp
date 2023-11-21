#include "production/weighted_branch.h"

#include <catch2/catch_all.hpp>

#include <calyx/grammar.h>

using namespace calyx;

TEST_CASE("Branch with multi choice test")
{
    Registry registry = Registry(Options(1234u, false));
    Options& ops = registry.getOptions();
    ErrorHolder errs;


    std::optional<WeightedBranch> branch = WeightedBranch::parse(
        {
            { ops.fromString("lithium"), 50.0 },
            { ops.fromString("silicon"), 30.0 },
            { ops.fromString("carbon"), 20.0 }
        },
        registry,
        errs
    );

    REQUIRE(branch.has_value());
    REQUIRE_FALSE(errs.hasError());

    std::optional<Expansion> exp = branch->evaluate(registry, ops, errs);

    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errs.hasError());

    REQUIRE(exp->flatten(ops) == ops.fromString("lithium"));
}

TEST_CASE("Results rougly match weightings")
{
    auto registry = Registry(Options(1234u));
    Options& ops = registry.getOptions();
    ErrorHolder errs;

    double fizzTimes = 700;
    double buzzTimes = 300;

    const String_t fizz = ops.fromString("fizz");
    const String_t buzz = ops.fromString("buzz");

    std::optional<WeightedBranch> branch = WeightedBranch::parse(
        {
            { fizz, fizzTimes },
            { buzz, buzzTimes }
        },
        registry,
        errs
    );

    REQUIRE(branch.has_value());
    REQUIRE_FALSE(errs.hasError());

    std::vector<Expansion> res;
    for (std::size_t i = 0; i < (fizzTimes + buzzTimes); i++)
    {
        std::optional<Expansion> exp = branch->evaluate(registry, ops, errs);
        res.push_back(*exp);
    }

    std::size_t totalFizz = std::ranges::count_if(res.begin(), res.end(), [&](Expansion& e) -> bool {
        return e.getTail()[0].getTail()[0].getTerm() == fizz;
    });
    std::size_t totalBuzz = std::ranges::count_if(res.begin(), res.end(), [&](Expansion& e) -> bool {
        return e.getTail()[0].getTail()[0].getTerm() == buzz;
    });

    REQUIRE_THAT(totalFizz, Catch::Matchers::WithinRel(fizzTimes, 0.05));
    REQUIRE_THAT(totalBuzz, Catch::Matchers::WithinRel(buzzTimes, 0.05));
}