#include <catch2/catch_all.hpp>

#include <memory>
#include <calyx/grammar.h>
#include <cycle.h>

using namespace calyx;

TEST_CASE("Cycle length 1 always returns 0th index")
{
    Options ops(1234u, false);
    ErrorHolder errs;
    std::optional<Cycle> cycle = Cycle::create(1, ops, errs);

    REQUIRE(cycle.has_value());
    REQUIRE_FALSE(errs.hasError());
    REQUIRE(0 == cycle->poll(ops));
    REQUIRE(0 == cycle->poll(ops));
    REQUIRE(0 == cycle->poll(ops));
}

TEST_CASE("Cycles refresh once fully consumed")
{
    Options ops(123u, false);
    ErrorHolder errs;
    const std::size_t count = 3;

    std::optional<Cycle> cycle = Cycle::create(count, ops, errs);

    REQUIRE(cycle);
    REQUIRE_FALSE(errs.hasError());

    std::vector<std::size_t> results;
    for (std::size_t i = 0; i < 2 * count; i++)
    {
        results.push_back(cycle->poll(ops));
    }


    std::ranges::sort(results.begin(), results.end());
    // contains the cycle values twice
    REQUIRE(results == std::vector<std::size_t> { 0, 0, 1, 1, 2, 2 });
}

TEST_CASE("Cycles are different each time")
{
    // by using a different seed to the "Cycles refresh once fully consumed" we verify
    // that the resulting cycle is indeed different each time
    Options ops(3211u, false);
    ErrorHolder errs;
    const std::size_t count = 3;
    std::optional<Cycle> cycle = Cycle::create(count, ops, errs);
    REQUIRE(cycle);
    REQUIRE_FALSE(errs.hasError());

    std::vector<std::size_t> firstResult;
    for (std::size_t i = 0; i < count; i++)
    {
        firstResult.push_back(cycle->poll(ops));
    }

    std::vector<std::size_t> secondResult;
    for (std::size_t i = 0; i < count; i++)
    {
        secondResult.push_back(cycle->poll(ops));
    }
    
    REQUIRE(firstResult != secondResult);
}