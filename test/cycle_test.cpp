#include <catch2/catch_all.hpp>

#include <memory>
#include <include/calyx.h>
#include <cycle.h>
#include <iostream>

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

    REQUIRE(results == std::vector<std::size_t> { 1, 2, 0, 2, 0, 1 });
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

    std::vector<std::size_t> results;
    for (std::size_t i = 0; i < 2 * count; i++)
    {
        results.push_back(cycle->poll(ops));
    }
    std::cout << "Called rng " << ops._rng << " times" << std::endl;
    
    REQUIRE(results == std::vector<std::size_t> { 1, 0, 2, 2, 1, 0 });
}

TEST_CASE("Shuffling shuffles")
{
    Options ops(321, false);
    ErrorHolder errs;
    const std::size_t count = 3;

    std::optional<Cycle> cycle = Cycle::create(count, ops, errs);

    REQUIRE(cycle);
    REQUIRE_FALSE(errs.hasError());

    cycle->shuffle(ops);

    const std::vector<std::size_t>& sequence = cycle->getSequence();

    REQUIRE(sequence == std::vector<std::size_t> { 2, 0, 1 });
}