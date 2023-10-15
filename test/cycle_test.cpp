#include <catch2/catch_all.hpp>

#include <memory>
#include <include/calyx.h>
#include <cycle.h>

using namespace calyx;

TEST_CASE("Cycle length 1 always returns 0th index")
{
    Options ops;
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
    Options ops(12345u, false);
    ErrorHolder errs;
    std::size_t count = 10;

    std::optional<Cycle> cycle = Cycle::create(count, ops, errs);

    REQUIRE(cycle);
    REQUIRE_FALSE(errs.hasError());

    std::vector<std::size_t> results;
    for (std::size_t i = 0; i < 2 * count; i++)
    {
        results.push_back(cycle->poll(ops));
    }

    REQUIRE(results == std::vector<std::size_t> { 7, 1, 8, 4, 2, 9, 3, 5, 6, 0, 9, 8, 7, 6, 2, 5, 4, 1, 3, 0 });
}

TEST_CASE("Cycles are different each time")
{
    // by using a different seed to the "Cycles refresh once fully consumed" we verify
    // that the resulting cycle is indeed different each time
    Options ops(53321321u, false);
    ErrorHolder errs;
    std::size_t count = 10;

    std::optional<Cycle> cycle = Cycle::create(count, ops, errs);

    REQUIRE(cycle);
    REQUIRE_FALSE(errs.hasError());

    std::vector<std::size_t> results;
    for (std::size_t i = 0; i < 2 * count; i++)
    {
        results.push_back(cycle->poll(ops));
    }

    REQUIRE(results == std::vector<std::size_t> { 7, 9, 6, 3, 4, 5, 0, 1, 8, 2, 2, 3, 4, 1, 7, 6, 9, 5, 8, 0 });
}

TEST_CASE("Shuffling shuffles")
{
    Options ops(3213211321u, false);
    ErrorHolder errs;
    std::size_t count = 10;

    std::optional<Cycle> cycle = Cycle::create(count, ops, errs);

    REQUIRE(cycle);
    REQUIRE_FALSE(errs.hasError());

    cycle->shuffle(ops);

    const std::vector<std::size_t>& sequence = cycle->getSequence();

    REQUIRE(sequence == std::vector<std::size_t> { 1, 4, 9, 3, 5, 6, 0, 8, 7, 2 });
}